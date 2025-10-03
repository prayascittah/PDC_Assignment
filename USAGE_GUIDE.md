# Usage Guide - Interactive Connected Components Programs

## Overview

Both the serial and parallel implementations now accept **user input** for:

- Number of vertices
- Number of edges
- Edge connections

Additionally, the parallel version allows you to specify the number of threads to use.

---

## 🚀 Quick Start

### Compile the Programs

```bash
cd "/home/prayascittah/Documents/Jayanth/BTech@IIITK/Sem5/CSS311 - PDC/Assignment-1"
make all
```

---

## 📝 Serial Version Usage

### Interactive Mode

```bash
./serial_cc
```

You'll be prompted to enter:

1. Number of vertices
2. Number of edges
3. Each edge (as two space-separated vertex numbers)

### Example Session

```
=================================================
   Serial Connected Components Algorithm B
   Based on Liu & Tarjan (2022)
=================================================

Enter the number of vertices: 4
Enter the number of edges: 3

Enter the edges (format: vertex1 vertex2):
Note: Vertices should be numbered from 0 to 3

Edge 1: 0 1
Edge 2: 1 2
Edge 3: 2 3

=================================================
   Processing Graph...
=================================================

Graph: 4 vertices, 3 edges

Connected Components (1 total):
Component 0: { 0 1 2 3 }

=================================================
   Performance Metrics
=================================================
Iterations: 2
Execution Time: 0.015 ms
Time Complexity: O(m log² n) where m = 3, n = 4

=================================================
   Algorithm completed successfully!
=================================================
```

### Using Input File

You can redirect input from a file:

```bash
./serial_cc < sample_input.txt
```

---

## ⚡ Parallel Version Usage

### Interactive Mode

```bash
./parallel_cc
```

You'll be prompted to enter:

1. Number of vertices
2. Number of edges
3. Each edge
4. **Number of threads to use**

### Example Session

```
=================================================
  Parallel Connected Components Algorithm B
  OpenMP Implementation
  Based on Liu & Tarjan (2022)
=================================================

Enter the number of vertices: 6
Enter the number of edges: 4

Enter the edges (format: vertex1 vertex2):
Note: Vertices should be numbered from 0 to 5

Edge 1: 0 1
Edge 2: 1 2
Edge 3: 3 4
Edge 4: 4 5

=================================================
   Thread Configuration
=================================================
System has 16 threads available
Enter number of threads to use (1-16): 4

=================================================
   Processing Graph...
=================================================

Graph: 6 vertices, 4 edges

--- Running SERIAL Version ---
Serial execution completed!
  Iterations: 2
  Time: 0.025 ms

--- Running PARALLEL Version (4 threads) ---
Parallel execution completed!
  Iterations: 2
  Time: 0.012 ms

Connected Components (2 total):
Component 0: { 0 1 2 }
Component 3: { 3 4 5 }

=================================================
   Performance Comparison
=================================================
Serial Time:    0.025 ms
Parallel Time:  0.012 ms
Speedup:        2.08x
Efficiency:     52.0%
Threads Used:   4

=================================================
   Algorithm completed successfully!
=================================================
```

---

## 📊 Sample Test Cases

### Test Case 1: Linear Graph

```
Vertices: 4
Edges: 3
Connections:
  0---1---2---3

Input:
4
3
0 1
1 2
2 3

Expected Output: 1 component {0, 1, 2, 3}
```

### Test Case 2: Two Separate Components

```
Vertices: 6
Edges: 4
Connections:
  0---1---2    3---4---5

Input:
6
4
0 1
1 2
3 4
4 5

Expected Output: 2 components {0,1,2} and {3,4,5}
```

### Test Case 3: Star Graph

```
Vertices: 5
Edges: 4
Connections:
      1
      |
  2---0---3
      |
      4

Input:
5
4
0 1
0 2
0 3
0 4

Expected Output: 1 component {0, 1, 2, 3, 4}
```

### Test Case 4: Cycle Graph

```
Vertices: 4
Edges: 4
Connections:
  0---1
  |   |
  3---2

Input:
4
4
0 1
1 2
2 3
3 0

Expected Output: 1 component {0, 1, 2, 3}
```

### Test Case 5: Disconnected Graph

```
Vertices: 6
Edges: 0
Connections: (none)

Input:
6
0

Expected Output: 6 components (each vertex isolated)
```

---

## 💡 Tips and Best Practices

### 1. Vertex Numbering

- **Always number vertices starting from 0**
- For n vertices, valid vertex numbers are: 0, 1, 2, ..., n-1
- The program will validate and reject invalid vertex numbers

### 2. Input Validation

The programs validate:

- ✅ Positive number of vertices
- ✅ Non-negative number of edges
- ✅ Valid vertex numbers in edge specifications
- ✅ Valid thread count (parallel version)

If you enter invalid data, you'll be prompted to re-enter.

### 3. Thread Selection (Parallel Version)

**General guidelines:**

- For small graphs (<100 vertices): Use 2-4 threads
- For medium graphs (100-1000 vertices): Use 4-8 threads
- For large graphs (>1000 vertices): Use 8-16 threads

**Note:** Using too many threads on small graphs may reduce performance due to overhead.

### 4. Using Input Files

Create a text file with your graph data:

**Example: `my_graph.txt`**

```
6
5
0 1
1 2
2 3
3 4
4 5
```

Run with input redirection:

```bash
./serial_cc < my_graph.txt
```

For parallel version with input file:

```bash
# Note: You'll still need to enter thread count interactively
./parallel_cc < my_graph.txt
```

### 5. Batch Testing

Create multiple test files and run them in sequence:

```bash
for file in test*.txt; do
    echo "Testing $file"
    ./serial_cc < "$file"
    echo "---"
done
```

---

## 🔍 Understanding the Output

### Serial Version Output

```
Connected Components (2 total):
Component 0: { 0 1 2 }
Component 3: { 3 4 5 }

Iterations: 2
Execution Time: 0.025 ms
Time Complexity: O(m log² n) where m = 4, n = 6
```

**Explanation:**

- **Component X**: The label (smallest vertex in the component)
- **{ ... }**: List of all vertices in that component
- **Iterations**: Number of main loop iterations
- **Execution Time**: Total runtime in milliseconds
- **Time Complexity**: Theoretical complexity with actual m and n values

### Parallel Version Output

```
--- Running SERIAL Version ---
  Iterations: 2
  Time: 0.025 ms

--- Running PARALLEL Version (4 threads) ---
  Iterations: 2
  Time: 0.012 ms

Speedup:        2.08x
Efficiency:     52.0%
```

**Explanation:**

- **Serial Time**: Baseline sequential execution time
- **Parallel Time**: Time with OpenMP parallelization
- **Speedup**: Serial Time / Parallel Time
- **Efficiency**: (Speedup / Threads) × 100%

**Good efficiency values:**

- 80-100%: Excellent (nearly linear speedup)
- 60-80%: Good (effective parallelization)
- 40-60%: Acceptable (some benefit from parallelization)
- <40%: Poor (overhead dominates)

---

## ⚙️ Advanced Usage

### 1. Testing with Different Thread Counts

```bash
# Test with 2, 4, 8 threads
for threads in 2 4 8; do
    echo "Testing with $threads threads"
    echo -e "10\n15\n0 1\n1 2\n2 3\n3 4\n4 5\n5 6\n6 7\n7 8\n8 9\n0 5\n2 7\n4 9\n1 6\n3 8\n$threads" | ./parallel_cc
    echo "---"
done
```

### 2. Generating Random Graphs

Create a script to generate random test graphs:

**`generate_graph.sh`**

```bash
#!/bin/bash
n=$1  # Number of vertices
m=$2  # Number of edges

echo $n
echo $m
for ((i=0; i<m; i++)); do
    u=$((RANDOM % n))
    v=$((RANDOM % n))
    echo "$u $v"
done
```

Usage:

```bash
chmod +x generate_graph.sh
./generate_graph.sh 100 200 > random_graph.txt
./serial_cc < random_graph.txt
```

### 3. Performance Benchmarking

Compare performance across different graph sizes:

```bash
#!/bin/bash
for n in 10 50 100 500 1000; do
    m=$((n * 2))
    echo "Testing n=$n, m=$m"
    ./generate_graph.sh $n $m | ./serial_cc
done
```

---

## ❓ Troubleshooting

### Problem: "Error: Vertex numbers must be between 0 and X"

**Cause:** You entered a vertex number outside the valid range.

**Solution:** Vertices must be numbered from 0 to (n-1) where n is the number of vertices.

### Problem: Slow execution on small graphs (parallel version)

**Cause:** Thread creation overhead exceeds computation time.

**Solution:** Use fewer threads (1-2) for small graphs.

### Problem: No speedup or negative speedup

**Possible causes:**

1. Graph is too small
2. Too many threads for available CPU cores
3. Memory bandwidth saturation

**Solution:**

- Use appropriately sized graphs for parallelization
- Match thread count to available CPU cores
- Monitor system resources

### Problem: Program hangs waiting for input

**Cause:** Missing input data or incorrect format.

**Solution:** Ensure you provide all required inputs in the correct order.

---

## 📚 Input Format Reference

### Complete Format

```
<number_of_vertices>
<number_of_edges>
<vertex1> <vertex2>    # Edge 1
<vertex1> <vertex2>    # Edge 2
...
<vertex1> <vertex2>    # Edge m
[<number_of_threads>]  # Only for parallel version
```

### Rules

1. **Vertices**: Must be integers from 0 to n-1
2. **Edges**: Two space-separated integers per line
3. **Threads**: Integer from 1 to system maximum (parallel only)
4. **No self-loops**: Edges where both vertices are the same are ignored
5. **Undirected**: Edge {u,v} is same as {v,u}

---

## 🎓 Educational Examples

### Example 1: Understanding Components

Try this sequence to understand how components are identified:

```bash
# Single component
echo -e "3\n2\n0 1\n1 2" | ./serial_cc

# Two components
echo -e "4\n2\n0 1\n2 3" | ./serial_cc

# Three components
echo -e "6\n3\n0 1\n2 3\n4 5" | ./serial_cc
```

### Example 2: Observing Speedup

Test parallelization benefits:

```bash
# Small graph (minimal speedup expected)
echo -e "10\n9\n0 1\n1 2\n2 3\n3 4\n4 5\n5 6\n6 7\n7 8\n8 9\n4" | ./parallel_cc

# Larger graph (better speedup expected)
./generate_graph.sh 1000 5000 > large.txt
cat large.txt | ./parallel_cc
```

---

## 📝 Summary

**Serial Version:**

- Simple interactive input
- Enter vertices, edges, and connections
- Get component results and performance metrics

**Parallel Version:**

- All serial features PLUS
- Choose number of threads
- Compare serial vs parallel performance
- See speedup and efficiency metrics

**Both versions:**

- Input validation
- Error handling
- Clear output formatting
- Performance measurement

---

For more information, see:

- `ASSIGNMENT_REPORT.md` - Complete algorithm documentation
- `README.md` - Project overview
- `ALGORITHM_EXAMPLES.md` - Visual examples

**Happy Computing! 🚀**
