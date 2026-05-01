# OS Simulator — Project Guide

## Overview

A terminal-based Operating System Simulator in C/C++ that mimics real OS behavior.
Each task is a **separate compiled executable** launched via `fork` + `exec` — no simple function calls allowed.

---

## File Structure

```
OS/
├── os.c                    # Main OS — boot, menu, resource manager, scheduler
├── process.h               # PCB struct, shared definitions
├── resource_manager.c/.h   # RAM/HDD/core tracking
├── kernel.c/.h             # Kernel mode, process table
├── ready_queue.c/.h        # Ready queue + scheduling (RR + multilevel)
├── scheduler.c/.h          # Context switching, state transitions
│
└── tasks/
    ├── calculator.c
    ├── notepad.c
    ├── clock.c
    ├── calendar.c
    ├── file_copy.c
    ├── file_move.c
    ├── file_delete.c
    ├── file_create.c
    ├── file_info.c
    ├── file_editor.c
    ├── text_search.c
    ├── minesweeper.c
    ├── music_sim.c
    ├── print_sim.c
    ├── ram_viewer.c
    ├── process_viewer.c
    ├── log_gen.c
    ├── rand_gen.c
    ├── timer_alarm.c
    └── auto_backup.c
```

Each task compiles to its own binary inside `tasks/`. The main OS execs these binaries as child processes.

---

## Build Order

### Phase 1 — Foundation (start here, nothing else works without it)

1. **`process.h`** — Define the PCB struct used everywhere:

```c
typedef struct {
    int pid;
    char name[64];
    int state;          // READY, RUNNING, BLOCKED, TERMINATED
    int ram_required;   // MB
    int hdd_required;   // MB
    int priority;
    int burst_time;
} PCB;
```

2. **`resource_manager.c`** — Track total/available RAM, HDD, cores.
   - Functions: `allocate_ram()`, `free_ram()`, `allocate_core()`, `free_core()`

3. **`os.c` boot sequence** — Parse `./OS 2 256 8` args, display OS name with `sleep()`, show task menu.

---

### Phase 2 — Process Creation and IPC

4. **Process launch via `fork` + `exec`** inside `os.c`:
   - Parent (OS) forks
   - Child execs the task binary (e.g., `./tasks/calculator`)
   - Use a **pipe** so the child can send its RAM/HDD requirements back to the OS
   - OS checks resources → sends `GRANT` or kills the child

5. **Each task opens in a new terminal** using:

```c
execlp("xterm", "xterm", "-e", "./tasks/calculator", NULL);
```

---

### Phase 3 — Scheduling

6. **`ready_queue.c`** — Linked list or array of PCBs
7. **Round Robin scheduler** — time quantum, cycle through ready queue
8. **Multilevel queue** — Level 0: Round Robin, Level 1: FCFS, Level 2: Priority (or your choice per level)
9. **Synchronization** — mutex around shared resource access (RAM table, ready queue)
   - Mutex/Lock
   - Semaphore
   - Condition variables

---

### Phase 4 — Tasks (build 5 at a time)

Start with the simplest and work up:

| Order | Tasks |
|---|---|
| First | Clock, Calendar, RAM viewer (auto-run, no user input needed) |
| Second | Calculator, Notepad with autosave (user interaction) |
| Third | File copy, File move, File delete, File info (background tasks) |
| Last | Minesweeper, Timer/alarm, Music sim, Print sim, Auto-backup |

**Template every task must follow:**

```c
// example: calculator.c
#include <stdio.h>
#include <string.h>

#define RAM_REQUIRED  10   // MB
#define HDD_REQUIRED   0   // MB

int main() {
    // 1. Report resource needs to parent OS via stdout pipe
    printf("RESOURCE_REQUEST %d %d\n", RAM_REQUIRED, HDD_REQUIRED);
    fflush(stdout);

    // 2. Wait for grant signal from OS
    char response[16];
    scanf("%s", response);
    if (strcmp(response, "GRANT") != 0) return 1;

    // 3. Actual task logic here
    // ...

    return 0;
}
```

---

### Phase 5 — Kernel Mode and Polish

- Kernel mode menu: list all running processes, kill by PID
- Minimize (send `SIGSTOP`) / Close (send `SIGKILL`) per process
- Shutdown: kill all child processes, free all resources, display goodbye message

---

## Compilation

```bash
# Compile all tasks first
gcc tasks/calculator.c  -o tasks/calculator
gcc tasks/notepad.c     -o tasks/notepad
gcc tasks/clock.c       -o tasks/clock
# ... repeat for all 20 tasks

# Compile the main OS (link pthread for multithreading)
gcc os.c resource_manager.c kernel.c ready_queue.c scheduler.c -o OS -lpthread

# Run with hardware specs: RAM(GB) HDD(GB) Cores
./OS 2 256 8
```

Or write a `Makefile` to compile everything at once.

---

## Member Responsibilities

| Member | Owns | Start With |
|---|---|---|
| Member 1 | Core OS, Resource Management, Boot, PCB, User/Kernel mode | `process.h` → `resource_manager.c` → boot in `os.c` |
| Member 2 | Ready Queue, Scheduling (RR + Multilevel), Context Switching, Synchronization | `ready_queue.c` → `scheduler.c` (use dummy PCBs to test independently) |
| Member 3 | All 20+ task programs, each as independent process with close/minimize | Start with: clock, calendar, calculator, file_copy, ram_viewer |

**Integration (all 3 together):**
- Connect task creation to resource manager
- Pass new processes to the ready queue
- Link scheduler with execution
- Implement interrupt handling (close/minimize signals)
- Final testing and debugging

---

## OS Concepts Covered

- Multitasking
- Context switching
- Resource allocation (RAM, HDD, CPU cores)
- User mode and Kernel mode
- Process creation (`fork` + `exec`)
- Threads (`pthreads`)
- EXEC commands
- Scheduling with mutual exclusion, semaphore, and condition variables
- Multilevel queue scheduling (different algorithm per level)

---

## Bonus

Implementing with a graphics library (e.g., `ncurses` for terminal UI, or a full GUI) earns bonus marks.

---

## Architecture Deep Dive

### How the OS starts

```
./OS 2 256 8
```

Standard C++ main — **not** `main(int ram, int hard, int core)`. Always use:

```cpp
int main(int argc, char* argv[]) {
    int ram   = atoi(argv[1]);
    int hdd   = atoi(argv[2]);
    int cores = atoi(argv[3]);
}
```

---

### How a task launches (full flow)

```
OS (parent)                          Task (child)
    |                                     |
    |-- fork() -------------------------->|
    |                                     |-- sends "REQUEST 10 0\n" via pipe
    |<-- reads pipe ----------------------|
    |-- checks resource_manager           |
    |   if OK:  sends "GRANT"            |
    |   if not: sends "DENY" + kills     |
    |                                     |-- receives GRANT → runs task logic
    |-- creates PCB, adds to queue        |
    |-- xterm opens for that child        |
```

Key point: **PCB is created after GRANT, not before.** The task sends its RAM/HDD requirements via pipe → OS reads them → checks availability → if granted, OS creates the PCB and stores those values in it.

---

### Who owns what data

| File | Owns |
|---|---|
| `process.h` | PCB struct, State enum |
| `resource.cpp` | total/available RAM, HDD, cores — global to the OS |
| `kernel.cpp` | process table (array of all PCBs), user/kernel mode switch |
| `ready_queue.cpp` | queue of PCBs waiting for CPU |
| `scheduler.cpp` | Round Robin + Multilevel logic, runs in a thread |
| `os.cpp` | `main()`, boot, menu, fork/exec, ties everything together |
| `tasks/*.cpp` | 20 independent executables |

---

### Exact build sequence

```
1. process.h           — PCB struct, State enum
2. resource.cpp        — hardware tracking (RAM/HDD/cores)
3. os.cpp skeleton     — boot screen, parse args, call resource init, menu shell
4. kernel.cpp          — process table, fork/exec one task to test pipe
5. ready_queue.cpp     — queue of PCBs
6. scheduler.cpp       — Round Robin + Multilevel scheduling in a thread
7. tasks/*.cpp         — 20 task executables, one at a time
```

## TASK CREATION

```
OS.cpp (parent)                    child process
      |                                  |
      |---- fork() -------------------> |
      |                                  |-- writes "REQUEST 10 0\n" to pipe
      |<-- reads pipe ------------------|
      |-- calls allocateRam(10)          |
      |   allocateHardDisk(0)            |
      |   allocateCore()                 |
      |                                  |
      | if all OK:                       |
      |-- writes "GRANT" to pipe ------> |-- execs xterm -e ./calculator
      |-- creates PCB                    |
      |                                  |
      | if not OK:                       |
      |-- writes "DENY" to pipe ------> |-- exits
      |-- shows "Not enough resources"   |
```

# Task Classification
```
Type 1 — Foreground (Interactive)
User must interact constantly. Goes BLOCKED when waiting for input.

Calculator
Notepad
File Editor
Text Search
Minesweeper
Type 2 — Background (Auto-finish)
Runs on its own, finishes automatically, no user input.

File Copy
File Move
File Delete
Print Simulation
Music Simulation
Auto Backup
Log Generator
Random Number Generator
Type 3 — System/Utility (Continuous)
Runs forever until killed, auto-updates.

Clock
Calendar
RAM Viewer
Process Viewer
Timer/Alarm
File Create
File Info
```