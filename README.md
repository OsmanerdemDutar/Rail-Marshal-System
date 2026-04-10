# Rail Marshal System

An intelligent **Rail Marshal System** prototype implemented in **C++11** using **manual data structures** such as a **doubly linked list-based mission stack**, **dynamic circular request queues**, and a **matrix-based emergency classification logic**.

This project was developed for **Hacettepe University BBM203 - Programming Assignment 1**.

---

## Project Overview

The system simulates an emergency coordination platform that manages two different types of requests:

- **SUPPLY** requests
- **RESCUE** requests

It also manages multiple response teams, each with a limited workload capacity. Incoming requests are prioritized according to their emergency scores, assigned to teams if capacity allows, and rolled back if an overload occurs.

The main goal of the project is to demonstrate low-level data structure implementation and queue/stack based mission handling without relying on STL containers for the core logic.

---

## Features

- Team initialization and per-team capacity assignment
- Separate queues for:
  - Supply operations
  - Rescue operations
- Emergency score calculation for request prioritization
- Workload-based mission assignment
- Stack-based mission history for each team
- Rollback mechanism when a team exceeds capacity
- Queue and team state printing commands
- Mission dispatch and cleanup support

---

## Data Structures Used

### 1. `RequestQueue`
A dynamic circular queue used to store pending requests.

Used for:
- `SUPPLY` queue
- `RESCUE` queue

Supports:
- enqueue
- dequeue
- peek
- remove by request ID
- dynamic resizing

### 2. `MissionStack`
A dynamic stack used to store requests assigned to a team.

Supports:
- push
- pop
- peek
- clear
- dynamic resizing

### 3. `Team`
Represents a disaster response team.

Stores:
- team ID
- max load capacity
- current workload
- active mission stack

### 4. `Request`
Represents either a:
- supply request
- rescue request

Includes emergency score and workload contribution calculations.

### 5. `QuakeAssistController`
The central controller that:
- parses commands
- manages queues
- manages teams
- assigns missions
- performs rollbacks
- prints system state

---

## Project Structure

```bash
Rail-Marshal-System/
│
├── data/
│   ├── input.txt
│   └── output.txt
│
├── src/
│   ├── MissionStack.cpp
│   ├── MissionStack.h
│   ├── QuakeAssistController.cpp
│   ├── QuakeAssistController.h
│   ├── Request.cpp
│   ├── Request.h
│   ├── RequestQueue.cpp
│   ├── RequestQueue.h
│   ├── Team.cpp
│   └── Team.h
│
├── BBM203_Fall_2025_Programming_Assignment_1.pdf
└── .gitignore
```

---

## How It Works

The system processes commands line by line from an input source.

Supported command types include:

- `INIT_TEAMS <numTeams>`
- `SET_TEAM_CAPACITY <teamID> <capacity>`
- `ADD_SUPPLY <id> <city> <supplyType> <amount> <emergencyLevel>`
- `ADD_RESCUE <id> <city> <numPeople> <risk> <emergencyLevel>`
- `REMOVE_REQUEST <id>`
- `HANDLE_EMERGENCY <teamID> <k>`
- `DISPATCH_TEAM <teamID>`
- `PRINT_QUEUES`
- `PRINT_TEAM <teamID>`
- `CLEAR`

---

## Emergency Score Logic

### Supply Request
Emergency score is computed as:

```text
emergencyLevel * 10 + min(amount, 50)
```

### Rescue Request
Emergency score is computed as:

```text
emergencyLevel * 10 + numPeople * riskMultiplier
```

Where:

- `LOW` → 1
- `MEDIUM` → 2
- `HIGH` → 3

---

## Workload Contribution Logic

### Supply Request
```text
workload = amount
```

### Rescue Request
```text
workload = numPeople * riskMultiplier
```

A request can only be assigned if the team's total workload stays within its maximum capacity.

---

## Rollback Mechanism

If a team exceeds its workload capacity while handling emergency requests:

- the current assignment process is stopped,
- all requests assigned in that mission are rolled back,
- requests are returned to their original queues,
- team workload is reset.

This ensures consistency and prevents partial invalid assignments.

---

## Example Commands

```txt
INIT_TEAMS 3
SET_TEAM_CAPACITY 0 40
SET_TEAM_CAPACITY 1 60
SET_TEAM_CAPACITY 2 30

ADD_SUPPLY S1 Ankara WATER 20 3
ADD_SUPPLY S2 Hatay FOOD 35 2
ADD_RESCUE R1 Adiyaman 5 HIGH 3
ADD_RESCUE R2 Malatya 3 MEDIUM 2

PRINT_QUEUES
HANDLE_EMERGENCY 0 2
PRINT_TEAM 0
DISPATCH_TEAM 0
CLEAR
```

---

## Build Instructions

Since the repository currently contains the class implementations under `src/`, you can compile them together with your own `main.cpp` driver file.

Example:

```bash
g++ -std=c++11 src/*.cpp main.cpp -o rail_marshal
```

Then run:

```bash
./rail_marshal
```

If your `main.cpp` reads from `data/input.txt`, you can redirect input like this:

```bash
./rail_marshal < data/input.txt
```

---

## Notes

- The project is implemented in **pure C++11**.
- Core queue and stack logic are implemented manually.
- The design focuses on algorithmic thinking and low-level data structure management.
- The repository includes both source files and sample `data/` files for testing.

---

## Possible Improvements

- Add a dedicated `main.cpp` entry point to make the project directly runnable
- Improve error handling for malformed commands
- Add unit tests
- Improve output formatting
- Add file-based input/output automation
- Replace raw pointers with safer memory management where allowed

---

## Course Information

This repository was developed as part of:

**Hacettepe University**  
**BBM203 - Software Laboratory I / Programming Assignment 1**  
**Fall 2025**

---
