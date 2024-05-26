# Ticket-System

This repository contains the code for a homework assignment from the SJTU ACM Class 2023. The project is a Ticket Management System, implemented in C++.

## Structure

```sh
Ticket-System
├─ .vscode
│  └─ settings.json
├─ CMakeLists.txt
├─ README.md
├─ main.cpp
├─ run-test
└─ utilities
   ├─ bp_tree
   │  ├─ CMakeLists.txt
   │  ├─ LRU.hpp
   │  ├─ bp_tree.hpp
   ├─ exceptions
   │  ├─ exceptions.hpp
   │  └─ sjtu_exceptions.hpp
   ├─ headers.hpp
   ├─ input_processer
   │  ├─ input_processer.cpp
   │  └─ input_processer.hpp
   ├─ log_system
   │  ├─ log_system.cpp
   │  └─ log_system.hpp
   ├─ map
   │  ├─ map.hpp
   │  └─ utility.hpp
   ├─ memory_river
   │  └─ memory_river.hpp
   ├─ pair
   │  └─ pair.hpp
   ├─ parameter_type
   │  └─ parameter_type.hpp
   ├─ priority_queue
   │  ├─ priority_queue.hpp
   │  └─ utility.hpp
   ├─ selling_system
   │  ├─ selling_system.cpp
   │  └─ selling_system.hpp
   ├─ time_system
   │  ├─ time_system.cpp
   │  └─ time_system.hpp
   ├─ train_manager
   │  ├─ train_manager.cpp
   │  └─ train_manager.hpp
   ├─ user_manager
   │  ├─ user_manager.cpp
   │  └─ user_manager.hpp
   └─ vector
      ├─ utility.hpp
      └─ vector.hpp
```

## Setup

1. **Clone the repository**:

    ```sh
    git clone https://github.com/Parsifal1986/Ticket-System.git
    ```

2. **Navigate to the project directory**:

    ```sh
    cd Ticket-System
    ```

3. **Build the project using CMake**:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage

Run the main program:

```sh
./build/code
```

## Requirements

- C++ Compiler
- CMake

## Contributors

- **Parsifal1986**: Initial work

## Plan to do

- [x] Finish pre work (B+ tree)

- [x] Finish manage system

- [x] bonus(LRU)

- [ ] more bonus(In the future? multi-thread)

For more information, visit the [project repository](https://github.com/Parsifal1986/Ticket-System).
