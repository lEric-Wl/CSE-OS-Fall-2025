# Simple Virtual Memory Simulation
#### Created by Alex Moses for CSE 4300

### MemoryManager.cpp
Basic class for a memory simulator. Uses vectors for physical memory, page table, and disk. Supports page replacement using a simple Clock implementation.

### MemorySimulation.cpp
Real-time utilization of the MemoryManager class. Allows allocation/deallocation, reading, writing, and printing info about pages.

### Usage
To compile the simulation, simply run:
```bash
c++ MemorySimulation.cpp MemoryManager.cpp -o MemorySimulation
```

### Presentation
https://docs.google.com/presentation/d/1x1X2-cfVryMw9sgM-Y9WeBlE7jDlHd6UByHzpAmIBfk
