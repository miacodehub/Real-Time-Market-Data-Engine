#include <atomic>
#include <chrono>
#include <cmath>
#include <deque>
#include <iostream>
#include <thread>

#ifdef __linux__
#include <pthread.h>
#endif

using Clock = std::chrono::high_resolution_clock;

struct Tick {
    double bid;
    double ask;
    int volume;
    Clock::time_point ts;
};

constexpr size_t BUFFER_SIZE = 2048;
constexpr size_t VOL_WINDOW = 100;

class RingBuffer {
public:
    bool push(const Tick& t) {
        size_t next = (head + 1) % BUFFER_SIZE;
        if (next == tail.load(std::memory_order_acquire))
            return false; // buffer full
        buffer[head] = t;
        head = next;
        return true;
    }

    bool pop(Tick& t) {
        if (tail.load(std::memory_order_acquire) == head)
            return false; // buffer empty
        t = buffer[tail];
        tail.store((tail + 1) % BUFFER_SIZE, std::memory_order_release);
        return true;
    }

private:
    Tick buffer[BUFFER_SIZE];
    size_t head{0};
    std::atomic<size_t> tail{0};
};

double computeVolatility(const std::deque<double>& returns) {
    if (returns.size() < 2) return 0.0;

    double mean = 0.0;
    for (double r : returns) mean += r;
    mean /= returns.size();

    double var = 0.0;
    for (double r : returns)
        var += (r - mean) * (r - mean);

    return std::sqrt(var / returns.size());
}

void pinThread(int cpu) {
#ifdef __linux__
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
#endif
}

int main() {
    RingBuffer buffer;
    std::atomic<bool> running{true};

    std::thread producer([&] {
        pinThread(0);

        double mid = 100.0;
        while (running) {
            mid += (std::rand() % 100 - 50) * 0.0005;

            Tick tick{
                mid - 0.01,
                mid + 0.01,
                10,
                Clock::now()
            };

            while (!buffer.push(tick)) {} // spin if full
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    });

    std::thread consumer([&] {
        pinThread(1);

        double pv = 0.0;
        double volumeSum = 0.0;
        double lastMid = 0.0;
        size_t tickCount = 0;
        std::deque<double> returns;

        while (running) {
            Tick tick;
            if (buffer.pop(tick)) {
                double mid = (tick.bid + tick.ask) * 0.5;
                double spread = tick.ask - tick.bid;

                pv += mid * tick.volume;
                volumeSum += tick.volume;

                if (lastMid > 0.0) {
                    double r = std::log(mid / lastMid);
                    returns.push_back(r);
                    if (returns.size() > VOL_WINDOW)
                        returns.pop_front();
                }
                lastMid = mid;

                auto latency = 
                    std::chrono::duration_cast<std::chrono::microseconds>(
                        Clock::now() - tick.ts).count();

                if (++tickCount % 1000 == 0) {
                    std::cout
                        << "VWAP=" << (pv / volumeSum)
                        << "  Spread=" << spread
                        << "  Volatility=" << computeVolatility(returns)
                        << "  Latency=" << latency << "us\n";
                }
            }
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    running = false;

    producer.join();
    consumer.join();
    return 0;
}
