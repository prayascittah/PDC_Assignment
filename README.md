# Connected Components Algorithm - CSS311 Assignment

## Overview

Implementation and analysis of **Algorithm B** for finding connected components in undirected graphs, based on the research paper:

> **"Simple Concurrent Connected Components Algorithms"**  
> by Sixue Cliff Liu and Robert Endre Tarjan (2022)  
> ACM Transactions on Parallel Computing, Vol. 9, No. 2, Article 9

## Algorithm Description

Algorithm B uses a label propagation approach with three main operations:

1. **Direct-Connect**: Updates parent pointers based on edge connectivity
2. **Shortcut**: Flattens the tree structure by path compression
3. **Alter**: Updates edges based on current parent values

The algorithm maintains a forest structure where each tree represents vertices in the same component being built.

## Project Structure

```
Assignment-1/
├── ASSIGNMENT_REPORT.md              # Complete assignment report
├── README.md                          # This file
├── Makefile                          # Build automation
├── serial_connected_components.cpp   # Serial implementation
├── parallel_connected_components.cpp # OpenMP parallel implementation
└── doc .pdf                          # Assignment description
```

## Compilation

### Prerequisites

- GCC/G++ compiler with C++17 support
- OpenMP library (usually included with GCC)
- Make utility

### Build Commands

```bash
# Compile both versions
make all

# Compile serial version only
make serial_cc

# Compile parallel version only
make parallel_cc

# Clean compiled files
make clean

# Show help
make help
```

### Manual Compilation

If you prefer to compile manually:

```bash
# Serial version
g++ -std=c++17 -O3 -o serial_cc serial_connected_components.cpp

# Parallel version
g++ -std=c++17 -O3 -fopenmp -o parallel_cc parallel_connected_components.cpp
```

## Running the Programs

### Serial Version

```bash
# Using make
make run-serial

# Or directly
./serial_cc
```

**Output includes:**

- Multiple test cases with different graph structures
- Component identification for each test
- Iteration count and execution time
- Scalability analysis

### Parallel Version

```bash
# Using make
make run-parallel

# Or directly
./parallel_cc
```

**Output includes:**

- Serial vs parallel performance comparison
- Speedup and efficiency metrics
- Thread scaling analysis (1, 2, 4, 8, 16 threads)
- Component identification results

### Run All Tests

```bash
make run-all
```

## Test Cases

### Test Case 1: Linear Graph

- **Structure**: Chain of vertices (1-2-3-4)
- **Purpose**: Test basic connectivity
- **Expected**: Single component

### Test Case 2: Multiple Components

- **Structure**: Two separate chains
- **Purpose**: Test component separation
- **Expected**: Two distinct components

### Test Case 3: Star Graph

- **Structure**: Central vertex connected to all others
- **Purpose**: Test hub connectivity
- **Expected**: Single component

### Test Case 4: Mixed Structure

- **Structure**: Complete graph, cycle, path, and star
- **Purpose**: Test various topologies
- **Expected**: Four components

### Test Case 5: Large Scale

- **Structure**: 1000 vertices with multiple components
- **Purpose**: Scalability testing
- **Expected**: Multiple components, performance metrics

## Algorithm Complexity

### Time Complexity

**Serial Version:**

- Per iteration: O(m + n) where m = edges, n = vertices
- Iterations: O(log² n) in worst case
- **Overall: O(m log² n)**

**Parallel Version (p processors):**

- Per iteration: O(m/p + n/p)
- With synchronization overhead: O(log p) per iteration
- **Overall: O((m log² n)/p + log² n × log p)**

### Space Complexity

- Parent array: O(n)
- Edge list: O(m)
- Temporary arrays: O(n)
- **Total: O(n + m)**

## Performance Metrics

The parallel implementation measures:

1. **Speedup**: T_serial / T_parallel
2. **Efficiency**: Speedup / num_threads × 100%
3. **Scalability**: Performance across different thread counts

### Expected Results

| Threads | Speedup  | Efficiency |
| ------- | -------- | ---------- |
| 1       | 1.00x    | 100%       |
| 2       | 1.8-1.9x | 90-95%     |
| 4       | 3.0-3.5x | 75-87%     |
| 8       | 5.0-6.0x | 62-75%     |
| 16      | 8-11x    | 50-68%     |

## OpenMP Parallelization Strategy

### 1. Initialization (Fully Parallel)

```cpp
#pragma omp parallel for num_threads(numThreads)
for (int i = 0; i < n; i++) {
    parent[i] = i;
}
```

### 2. Direct-Connect (Parallel with Reduction)

```cpp
#pragma omp parallel for reduction(||:changed) num_threads(numThreads)
for (int i = 0; i < edges.size(); i++) {
    // Update parent with atomic operations
    #pragma omp atomic write
    parent[v] = minVal;
}
```

### 3. Shortcut (Two-Phase Parallel)

```cpp
// Phase 1: Parallel read
#pragma omp parallel for
for (int i = 0; i < n; i++) {
    oldParent[i] = parent[i];
}

// Phase 2: Parallel write
#pragma omp parallel for reduction(||:changed)
for (int i = 0; i < n; i++) {
    parent[i] = oldParent[oldParent[i]];
}
```

### 4. Alter (Parallel Edge Processing)

```cpp
#pragma omp parallel for
for (int i = 0; i < edges.size(); i++) {
    // Update or mark edges independently
}
```

## Key Implementation Features

### Serial Version (`serial_connected_components.cpp`)

- Pure C++ implementation without parallelization
- Clear demonstration of algorithm steps
- Comprehensive test suite
- Performance measurement and analysis

### Parallel Version (`parallel_connected_components.cpp`)

- OpenMP parallelization with configurable thread count
- Atomic operations for thread-safe updates
- Reduction clauses for synchronization
- Direct serial vs parallel comparison
- Thread scaling analysis

## Assignment Compliance

This implementation fulfills all requirements:

### MODULE-I (Serial Implementation)

- ✅ Problem objective and description
- ✅ Pseudocode and algorithm details
- ✅ Solution demonstration with test cases
- ✅ C++ serial code implementation
- ✅ Time complexity analysis

### MODULE-II (Parallel Implementation)

- ✅ Identification of parallelizable blocks
- ✅ OpenMP parallel pseudocode
- ✅ Parallel solution demonstration
- ✅ OpenMP C++ implementation
- ✅ Parallel time complexity analysis
- ✅ Speedup and efficiency measurements

## References

1. **Primary Paper:**

   - Sixue Cliff Liu and Robert Endre Tarjan. "Simple Concurrent Connected Components Algorithms." ACM Trans. Parallel Comput. 9, 2, Article 9 (August 2022).
   - DOI: https://doi.org/10.1145/3543546

2. **Implementation Reference:**
   - GitHub: https://github.com/mehanix/Algorithm-B-OpenMP
3. **OpenMP Documentation:**
   - OpenMP Architecture Review Board. "OpenMP Application Programming Interface, Version 5.0" (2018).

## Usage Examples

### Example 1: Quick Test

```bash
make clean
make all
make run-serial
```

### Example 2: Parallel Performance Analysis

```bash
make parallel_cc
./parallel_cc > results.txt
```

### Example 3: Custom Thread Count

Modify the `numThreads` variable in `parallel_connected_components.cpp`:

```cpp
int numThreads = 16;  // Set desired thread count
```

## Troubleshooting

### OpenMP Not Found

```bash
# Install on Ubuntu/Debian
sudo apt-get install libomp-dev

# Install on macOS
brew install libomp
```

### Compilation Errors

```bash
# Check GCC version (need 7.0+)
g++ --version

# Update if needed
sudo apt-get update
sudo apt-get install g++
```

### Runtime Issues

- Ensure sufficient system resources
- For large graphs, increase stack size: `ulimit -s unlimited`
- Monitor thread count: `echo $OMP_NUM_THREADS`

## Contributing

This is an academic assignment. The implementation is based on:

- Research paper by Liu & Tarjan (2022)
- Reference implementation: https://github.com/mehanix/Algorithm-B-OpenMP

## Author

**Course**: CSS311 - Parallel and Distributed Computing  
**Institution**: IIIT Kottayam  
**Semester**: 5  
**Academic Year**: 2024-25

## License

This code is provided for educational purposes as part of academic coursework.

---

For detailed analysis and results, see `ASSIGNMENT_REPORT.md`
