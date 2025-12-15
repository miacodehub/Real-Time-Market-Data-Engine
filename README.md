# Real-Time-Market-Data-Engine
A low-latency, real-time market data analytics engine implemented in modern C++. The system simulates a live tick feed and processes streaming data using a lock-free single-producer/single-consumer ring buffer to compute real-time financial metrics.


# Key Features

- Real-time tick ingestion
- Lock-free SPSC ring buffer
- Deterministic producer/consumer threads
- Live VWAP computation
- End-to-end latency measurement (µs)
- Mid-price & spread calculation
- Rolling log-return volatility
- Microsecond-level end-to-end latency tracking
- Optional CPU core pinning (Linux)

 # Technologies implemented

C++17
Standard Library:
   - std::thread
   - std::atomic
   - std::chrono
   - std::deque
POSIX Threads (Linux only)
pthread_setaffinity_np (optional CPU pinning)
Compiler
GCC ≥ 9 / Clang ≥ 10 recommended

# Possible Extensions

Historical CSV replay at original timestamps
Multi-symbol sharding
Signal generation (VWAP cross, volatility spike)
p99 / p999 latency tracking

# Steps to run

1. Make sure you have a C++17 compiler installed:
   ```
   g++ --version
   ```
2. Compile the program:
   ```
   g++ -O2 -std=c++17 main.cpp -pthread -o market_engine
   ```
3. Run the executable:
   ```
   ./market_engine
   ```
   
# Output
<img width="816" height="313" alt="image" src="https://github.com/user-attachments/assets/14e59359-cdcc-43a1-8a80-2c5b2ebe93fc" />




