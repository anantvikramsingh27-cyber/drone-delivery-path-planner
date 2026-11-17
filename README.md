# 🚁 Project 45: Drone Delivery Path Planner

## 📌 Project Overview
The Drone Delivery Path Planner is a console-based application developed using the C programming language.

It simulates a drone delivery system that selects packages based on priority and weight capacity, plans a delivery route, and generates a flight report.

## ✨ Features

- Accepts multiple package details from the user.
- Stores package coordinates using 2D arrays.
- Uses structures to manage package information.
- Checks drone weight capacity before selecting packages.
- Prioritizes packages based on their priority level.
- Plans delivery sequences using the Nearest Neighbour Algorithm.
- Calculates total delivery distance.
- Estimates flight time and fuel consumption.
- Uses dynamic memory allocation for package storage.

## 🛠️ Technologies Used

- Programming Language: C
- Compiler: GCC / Nimbus
- Concepts: Structures, Functions, Pointers, 2D Arrays, Loops, Dynamic Memory Allocation.

## ⚙️ How It Works

1. The user enters the number of packages.
2. The program collects coordinates, package weight and priority.
3. Packages are selected according to priority and drone capacity.
4. The route planner selects the nearest available delivery location.
5. The drone returns to its starting point (Depot).
6. A final report displays the delivery sequence, distance, time and estimated fuel usage.

## 📥 How to Run

Compile the program:

```bash
gcc drone_delivery.c -o drone_delivery
```

Run on Windows:

```bash
drone_delivery.exe
```

Run on Linux/macOS:

```bash
./drone_delivery
```

## 📚 Concepts Demonstrated

- 2D Arrays
- Structures
- Functions
- Pointers
- Dynamic Memory Allocation
- Loops and Conditional Statements
- Greedy Route Planning Algorithm

## ⚠️ Note

The route planner uses a Nearest Neighbour greedy approach. It provides a practical route simulation but does not guarantee the globally shortest route.

Fuel and flight-time values are estimates based on user-provided inputs.

## 👨‍💻 Project Information

**Project Name:** Drone Delivery Path Planner  
**Project Number:** 45  
**Language:** C  
**Environment:** Bytexl Nimbus

---

Developed as an academic C programming project.
