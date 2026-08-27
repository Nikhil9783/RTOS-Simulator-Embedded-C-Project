

<div align="center">
  
# ⚡ Embedded RTOS Simulator in C  
**A Project for Real-Time OS Concepts, Task Scheduling, and Embedded Systems**  

</div>

---

> Implements a simulated priority-based scheduler with round-robin selection, message queues (strings), semaphores, simulated UART, shell CLI, timer and interrupt simulation, logging, and diagnostics — no hardware required.

---

## Table of Contents

- [Why this project](#why-this-project)  
- [Features](#features)  
- [Architecture & design](#architecture--design)  
- [Build & run](#build--run)  
- [Shell / Command Line Interface](#shell--command-line-interface)  
- [Example session](#example-session)  
- [Tests & Continuous Integration](#tests--continuous-integration)  
- [Limitations & future work](#limitations--future-work)  
- [License](#license)  


---


## Why this project

This repository is built to teach the core concepts of operating systems and real-time embedded software through a hands-on, incremental approach:

- Learn how kernels schedule tasks and handle concurrency
- Understand interrupts, ISRs and how they wake tasks
- Practice writing firmware-style C (no OS abstractions)
  
---

## Features

- **Kernel fundamentals**
  - Task Control Block (TCB) style `Task` structure
  - Task states: `READY`, `RUNNING`, `BLOCKED`, `SLEEPING`, `TERMINATED`
  - Priority-based scheduler with round-robin tie-break
  - Idle task + CPU usage monitoring

- **IPC & sync**
  - Message queue (string messages)
  - Binary semaphore (mutex-like behavior)

- **Peripherals (simulated)**
  - UART simulation (TX/RX circular buffers)
  - Timer simulation with `timer_isr()` callback
  - Interrupt vector table (`registerInterrupt` / `triggerInterrupt`)

- **User Interface**
  - CLI shell running as a task with commands: `help`, `list`, `send`, `pause`, `resume`, `kill`, `log show`, `log clear`, `set threshold`, `status`, `trigger_interrupt`, etc.

- **Diagnostics**
  - Log buffer for sensor data
  - `status` command shows queue usage, tick count, task counts, CPU usage

---

## Architecture & design

High-level components:

- **main loop**: `tick()` → `runScheduler()` → `sleep/usleep` (simulated)
- **scheduler**: selects the highest-priority READY task and runs it
- **tick()**: advances sleep counters and triggers the timer ISR
- **interrupt module**: simple vector table and API to register/trigger ISRs
- **IPC**: circular queue for string messages
- **Shell**: user-facing CLI to control runtime


```plaintext
+--------------------------------------------------+
|                    Shell Task                   |
| (User Commands: list, send, status, etc.)         |
+--------------------------------------------------+
               |              |  
               v              v  
+---------------------+   +---------------------+
|   Network Listener  |   |    LED Blinker      |
| (Simulated events)  |   | (Status LED toggle) |
+---------------------+   +---------------------+
               |              ^
               v              |
+---------------------+   +---------------------+
|    Data Logger      |<--| Message Queue       |
| (Logs temperature)  |   | (IPC mechanism)     |
+---------------------+   +---------------------+
               ^
               |
+---------------------+
| Interrupt Handlers  |
| (Timer, Button ISR) |
+---------------------+
```
---

## Build & Run

### Prerequisites

- **Linux / macOS**: `gcc`, `make`
- Windows: MSYS2 with `base-devel`, `gcc`, `make`

### Build

```bash
git clone https://github.com/Nikhil9783/RTOS-Simulator-Embedded-C-Project.git
cd RTOS-Simulator-Embedded-C-Project
make clean
make
./rtos
```

## Shell / Command Line Interface

help                       - Show help
list                       - List all tasks and their state
send <message>             - Send string message to queue
log show                   - Show in-memory log
log clear                  - Clear the log buffer
 trigger <taskId>           - Manually set a task READY
 trigger interrupt <id>     - Call registered ISR (simulate button/timer)
status                     - Print detailed system status
clear                      - Clear terminal screen

### Example session
``` bash
[SHELL] >>> help
[SHELL] >>> send Hello from RTOS
[SHELL] >>> log show
[SHELL] >>> trigger_interrupt 0
[SHELL] >>> status
```
### Example output
``` bash
==== RTOS Boot ====
[KERNEL] Task Created: ID=0 | Name=NetListener | Priority=1
[KERNEL] Task Created: ID=1 | Name=Logger | Priority=1
[KERNEL] Task Created: ID=2 | Name=LED_Blinker | Priority=1
[KERNEL] Task Created: ID=3 | Name=ShellTask | Priority=1
[KERNEL] Task Created: ID=4 | Name=IdleTask | Priority=0

[TICK 0]
[SCHEDULER] Running: NetListener (Priority: 1)
[NET] Sent: Net Event #1

```
---

## Tests & Continuous Integration

This project uses **GitHub Actions** for automated Continuous Integration (CI).  
Every time code is pushed or a pull request is opened, GitHub automatically:

- **Builds** the RTOS Simulator on a fresh environment  
- **Runs** the unit-test suite on Ubuntu and Windows
- **Runs** a short simulator smoke test on Linux

---

### 📌 CI Status  
![CI Status](https://github.com/Nikhil9783/RTOS-Simulator-Embedded-C-Project/actions/workflows/ci.yml/badge.svg)

---

### 📂 How It Works  
- The workflow file is located in `.github/workflows/ci.yml`  
- It runs the build using GCC inside a Linux runner  
- Any compilation failure will **break the build** and mark the commit as ❌  

---

### ▶️ Running Locally  
If you want to check the build on your own system:  

```bash
# Build the RTOS Simulator
gcc -o rtos src/*.c -Iinclude

# Run the simulator
./rtos
```

---

## Limitations & future work

### Limitations

- This is a simulation running on a host OS — not a real microcontroller. Timing and power states are approximated.

- No actual context switching of CPU registers is performed (we call task functions directly).

- No stack/heap per task simulation (can be added later).

### Planned / nice-to-have

- Implement nested interrupts and interrupt priorities

- Task stack usage simulation and simulated per-task memory limits

## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for the original copyright notice and full terms.

This repository is published under the original MIT license with repository-specific documentation and configuration updates.

### Planned improvements

- Add log rotation for long-running sessions

- Add integration tests for the interactive shell and runtime logging

- Port demo tasks to run on a microcontroller (STM32/ESP32) as follow-up project
