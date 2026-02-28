# C++ Client-Server Inventory Management System

A multi-threaded client-server application written in C++ for managing shared inventory items.

## Features
- Client-server architecture
- TCP socket communication
- Concurrent client handling
- Thread-safe inventory management
- Borrowing, returning, and item availability tracking

## Technologies Used
- C++
- TCP Sockets
- Multithreading
- Mutex
- Condition Variables
- CMake

## Project Structure
- `server.cpp` – server logic
- `client.cpp` – client logic
- `InventoryManager.*` – inventory management functionality
- `Item.*` – item representation

## How to Run
1. Build the project using CMake.
2. Run the server.
3. Run one or more clients.
4. Send commands to manage and track inventory items.

## Notes
This project was developed as an academic project to demonstrate client-server communication, concurrency, and system design in C++.
