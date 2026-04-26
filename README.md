# Semiactive Suspension Wrapper

Runtime wrapper for semi-active suspension control.  
This process runs four wheel control loops, processes sensor traffic over ZMQ, and publishes wheel setpoints back out over ZMQ.

For algorithm-specific docs, see [`algorithm/README.md`](algorithm/README.md).

## Platform Support

- Linux
- macOS

## Typical Setup and Run Flow

1. Install dependencies:
   - `./setup.sh`
2. Build (and auto-run) the wrapper:
   - `./compile.sh`
3. Run an existing build directly:
   - `./run.sh`

## Repository File Overview

## Bash Files

### `setup.sh`
- Detects Linux/macOS.
- Installs required dependencies (Homebrew + apt packages where applicable).
- Installs ZeroMQ and common toolchain dependencies.

### `run.sh`
- Ensures `logs/` exists.
- Runs `./build/SemiActiveWrapper` if present.
- Prints a reminder to build first if the binary is missing.

### `compile.sh`
- Ensures `build/` and `logs/` exist.
- Compiles `src/*.cpp` and `algorithm/*.cpp` into `build/SemiActiveWrapper`.
- Links against ZMQ and pthread.
- Runs the built binary after a successful compile.

### `pim-semiactive.service`
- Systemd service for deploying the wrapper on the Pi.
- Runs `/home/baja/piM/semiactive-wrapper/run.sh`.
- Configured to restart automatically.

## C++ Files

### `src/main.cpp`
- Program entrypoint.
- Creates shared memory, shared sensor/output objects, logger, ZMQ updater, and four wheel wrappers.
- Launches one thread per wheel plus one ZMQ thread.

### `src/Configs.hpp`
- Central runtime constants:
  - wheel subject IDs and unique names,
  - update frequency and timeout threshold,
  - ZMQ input/output endpoints,
  - setpoint message type.

### `src/Logger.hpp`
- Thread-safe logger used by all runtime threads.
- Supports timestamped file logging and console logging.
- Includes helpers for startup, timeout, critical failure, and periodic stats logs.

### `src/Utils.hpp`
- Shared data structures and algorithm interface:
  - `SharedAlgorithmMemory`,
  - `Sensor`,
  - `ZMQSensorData`,
  - `ZMQOutput`,
  - declarations for `setup`, `update`, `emergency`.

### `src/WheelWrapper.hpp`
- Per-wheel control loop wrapper.
- Calls algorithm hooks (`setup`, `update`, `emergency`) for one subject ID.
- Enforces frame timing and timeout handling; escalates to emergency mode on repeated failures.

### `src/ZMQUpdater.hpp`
- ZMQ bridge between external traffic and internal shared state.
- Subscribes to inbound sensor messages, parses JSON, updates `ZMQSensorData`.
- Publishes outbound wheel setpoint messages from `ZMQOutput`.

### `algorithm/algorithm.cpp`
- User-editable control logic implementation.
- Defines required algorithm hooks:
  - `setup(...)`
  - `update(...)`
  - `emergency(...)`

## Misc Helpers

### `misc/zmq_fakesensor.py`
- Publishes randomized sample sensor-like messages to `ipc:///tmp/cyphal_in` for debugging.

### `misc/zmq_monitor.py`
- Monitors both `cyphal_in` and `cyphal_out`.
- Optional `--subject-id` (or `-s`) filtering to view specific message streams.

### `misc/zmq_print.py`
- Subscriber that prints raw messages from `ipc:///tmp/cyphal_in`.

### `misc/zmq_send.py`
- Publisher for manually sending messages to `ipc:///tmp/cyphal_out`.
