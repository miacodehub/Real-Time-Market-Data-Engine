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



Build

Single-file, no external dependencies
