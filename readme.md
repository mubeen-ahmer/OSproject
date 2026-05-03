# OS Simulator — Project Guide

## Overview

A terminal-based Operating System Simulator in C/C++ that mimics real OS behavior.
Each task is a **separate compiled executable** launched via `fork` + `exec` — no simple function calls allowed.


Each task compiles to its own binary inside `tasks/`. The main OS execs these binaries as child processes.



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


## -----------------------------------------------------
### Must read 
### ----------------------------------------------------
```
level 0 tasks : lets say we have one core and we open multiple tasks all will be opened and each will get cpu for a specific time 
mean the context switching will happen


level 1 tasks : if we have one core and we open a task it will be opened but if you open another one it will be added in the ready queue and will be  starving and will be waiting for the previous one to be closed 
lets say when fisrts one was running and we clicked multiple tasks to be open (none will  open) these will get cpu accrding to FCFs rule like the one you clicked at start will be opened and then the other and so on

level 2 tasks: if we have one core and we open a task it will be opened but if you open another one it will be added in the ready queue and will be  starving and will be waiting for the previous one to be closed 
lets say when fisrts one was running and we clicked multiple tasks to be open (none will  open) these will get cpu accrding to the priority rule like the one which has the lowest number , highest priority will be opened and so on



```

## --------------------------------------------------------
## MEMBER 2: SCHEDULING, ALGORITHMS & SYNCHRONIZATION GUIDE
## --------------------------------------------------------
This section explains exactly how the core OS mechanics function. If the professor asks how your part of the project works, use these points!

### 1. The Multilevel Ready Queue (`ready_queue.cpp`)
When a task is launched, it is assigned to one of three queues:
* **Level 0 (Foreground):** Uses **Round Robin**. (Calculator, Notepad, WordCount, MineSweeper)
* **Level 1 (Background):** Uses **FCFS**. (Music Sim, Print Sim, File Copy/Move/Delete)
* **Level 2 (System):** Uses **Priority Scheduling**. (Clock, Calendar, Viewers, Log/Random/Password Gens)

**How they get CPU:** The scheduler ALWAYS checks Level 0 first. If it's empty, it checks Level 1. If Level 1 is empty, it checks Level 2. Lower levels suffer from *Starvation* if higher levels are always full.

### 2. True Multi-Processing (Cores)
Instead of just faking cores with a counter, the OS boots exactly `N` `schedulerThread`s to represent `N` physical CPU Cores. 
* If you have 4 Cores, 4 threads pull tasks from the queue simultaneously.
* This solves the FCFS blocking issue! If a Level 1 task (like `fileCopy`) waits for user input, it ties up exactly 1 Core. The other 3 Cores continue to run your Foreground and System tasks perfectly. 

### 3. Context Switching (`scheduler.cpp`)
Context Switching is handled using Linux POSIX signals:
* **Context Switch IN:** The OS sends `SIGCONT` to a process to give it the CPU.
* **Context Switch OUT:** For Round Robin tasks (Level 0), the Core lets the task run for `0.2` seconds. When the quantum expires, it sends `SIGSTOP` to completely freeze the process, saves its state, throws it back in the Ready Queue, and picks up the next task. 
* This rapid `0.2s` swapping is what allows 5 Calculators to seemingly run at the same time on only 1 Core!

### 4. Synchronization Primitives (`scheduler.cpp`)
All three required primitives are used to actively gate execution:
* **Semaphore (`gTaskSemaphore`):** This is a counting semaphore that tracks how many tasks are ready. When the queue is empty, the Cores hit `sem_wait()` and physically go to Sleep, using 0% CPU. When a task is launched, `sem_post()` wakes up exactly one Core to handle it.
* **Mutex (`gSchedMutex` & `gMutex`):** Locks the linked lists and resource variables so multiple Cores don't crash into each other when trying to pull from the same queue or update RAM simultaneously.
* **Condition Variable (`gQueueCond`):** Used as a secondary wakeup mechanism inside the scheduler to prevent spurious thread wakeups if the queue somehow drains too fast.

### 5. Kernel Mode (Simulated Interrupts)
Because we cannot automatically detect when a separate Linux `xterm` window is waiting for keyboard input, we simulate Hardware Interrupts manually. 
* By going into Kernel Mode (Option 21) and selecting **Pause (SIGSTOP)**, you manually interrupt a process, force it into the `Blocked` state, and free up its CPU Core. 
* Using **Resume (SIGCONT)** moves it back to the `Ready` queue.



#### ------------------------------------------------------------

### how to show  (Ready → Running → Blocked → Ready)

#### ------------------------------------------------------------


## Step 1: Prove "Ready" and "Running"
Boot the OS with 1 Core.
Launch Calculator and Notepad.
Type 21 to go into Kernel Mode, then type 1 to List all processes.
Look at the State: line for both tasks.
You will see one task says State: Running (because it currently holds the Core).
You will see the other task says State: Ready (because it is waiting in the queue).
(If you check the list multiple times, you will actually see them swapping states as the Round Robin scheduler passes the Core back and forth!)

## Step 2: Prove "Blocked"

Stay in Kernel Mode.
Type 3 to Pause a process, and type in the PID of the Calculator.
Type 1 to List all processes again.
Look at the State: line for Calculator. It will now explicitly say State: Blocked!
Explain to your professor: "Because my OS cannot natively intercept keyboard hardware interrupts from a separate Linux window, I manually simulated an interrupt by forcing the task into 
the Blocked state using a kernel signal."

## Step 3: Prove the cycle back to "Ready/Running"

Type 4 to Resume the process, and type in the PID of the Calculator.
Type 1 to List all processes one last time.
You will see Calculator is no longer Blocked. The OS moved it back into the Ready Queue, and it will now flip back and forth between Ready and Running again.
