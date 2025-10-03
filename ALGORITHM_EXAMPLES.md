# Algorithm B - Visual Examples and Walkthrough

## Connected Components Algorithm - Step-by-Step Guide

This document provides visual examples of how Algorithm B works on different graph structures.

---

## Example 1: Simple Linear Graph

### Input Graph

```
Vertices: {0, 1, 2, 3}
Edges: {0-1}, {1-2}, {2-3}

Visual:
  0 --- 1 --- 2 --- 3
```

### Initial State

```
parent = [0, 1, 2, 3]

Forest representation:
  0    1    2    3
  |    |    |    |
  0    1    2    3  (each vertex points to itself)
```

### Iteration 1

#### Step 1: Direct-Connect

Process each edge and update parents:

- Edge {0,1}: 0 < 1, so parent[1] = min(parent[1], 0) = min(1, 0) = 0
- Edge {1,2}: 1 < 2, so parent[2] = min(parent[2], 1) = min(2, 1) = 1
- Edge {2,3}: 2 < 3, so parent[3] = min(parent[3], 2) = min(3, 2) = 2

```
parent = [0, 0, 1, 2]

Forest:
     0
     |
     1
     |
     2
     |
     3
```

#### Step 2: Shortcut

Set parent[v] = parent[parent[v]]:

- parent[0] = parent[parent[0]] = parent[0] = 0 (no change)
- parent[1] = parent[parent[1]] = parent[0] = 0 (no change)
- parent[2] = parent[parent[2]] = parent[1] = 0
- parent[3] = parent[parent[3]] = parent[2] = 1

```
parent = [0, 0, 0, 1]

Forest:
     0
    /|\
   0 1 2
       |
       3
```

#### Step 3: Alter

Update edges based on parents:

- Edge {0,1}: parent[0]=0, parent[1]=0 → same parent → DELETE
- Edge {1,2}: parent[1]=0, parent[2]=0 → same parent → DELETE
- Edge {2,3}: parent[2]=0, parent[3]=1 → different → KEEP as {0,1}

```
Edges after alter: {{0,1}}
```

### Iteration 2

#### Step 1: Direct-Connect

- Edge {0,1}: 0 < 1, so parent[1] = min(0, 0) = 0 (no change)

```
parent = [0, 0, 0, 1]
```

#### Step 2: Shortcut

- parent[3] = parent[parent[3]] = parent[1] = 0

```
parent = [0, 0, 0, 0]

Forest:
     0
    /|\\
   0 1 2 3  (all point to 0)
```

#### Step 3: Alter

- Edge {0,1}: parent[0]=0, parent[1]=0 → DELETE

```
Edges: {} (empty)
```

### Final Result

```
parent = [0, 0, 0, 0]
Component 0: {0, 1, 2, 3}
```

---

## Example 2: Two Separate Components

### Input Graph

```
Vertices: {0, 1, 2, 3, 4}
Edges: {0-1}, {1-2}, {3-4}

Visual:
  0 --- 1 --- 2       3 --- 4
  (Component A)       (Component B)
```

### Initial State

```
parent = [0, 1, 2, 3, 4]
```

### Iteration 1

#### Direct-Connect

- Edge {0,1}: parent[1] = 0
- Edge {1,2}: parent[2] = 1
- Edge {3,4}: parent[4] = 3

```
parent = [0, 0, 1, 3, 3]

Forest:
  0       3
  |      / \
  1     3   4
  |
  2
```

#### Shortcut

- parent[2] = parent[1] = 0
- parent[4] = parent[3] = 3

```
parent = [0, 0, 0, 3, 3]

Forest:
    0         3
   /|\       / \
  0 1 2     3   4
```

#### Alter

- {0,1}: DELETE
- {1,2}: DELETE
- {3,4}: DELETE

```
Edges: {} (all deleted, components are flat)
```

### Final Result

```
parent = [0, 0, 0, 3, 3]
Component 0: {0, 1, 2}
Component 3: {3, 4}
```

---

## Example 3: Star Graph

### Input Graph

```
Vertices: {0, 1, 2, 3, 4}
Edges: {0-1}, {0-2}, {0-3}, {0-4}

Visual:
      1
      |
  2---0---3
      |
      4
```

### Initial State

```
parent = [0, 1, 2, 3, 4]
```

### Iteration 1

#### Direct-Connect

All edges connected to vertex 0:

- Edge {0,1}: 0 < 1, parent[1] = 0
- Edge {0,2}: 0 < 2, parent[2] = 0
- Edge {0,3}: 0 < 3, parent[3] = 0
- Edge {0,4}: 0 < 4, parent[4] = 0

```
parent = [0, 0, 0, 0, 0]

Forest: (already flat)
     0
   / | \ \
  1  2  3 4
```

#### Shortcut

No changes (already flat)

#### Alter

All edges have same parent → DELETE all

```
Edges: {}
```

### Final Result

```
parent = [0, 0, 0, 0, 0]
Component 0: {0, 1, 2, 3, 4}
Iterations: 1 (optimal!)
```

---

## Example 4: Cycle Graph

### Input Graph

```
Vertices: {0, 1, 2, 3}
Edges: {0-1}, {1-2}, {2-3}, {3-0}

Visual:
    0---1
    |   |
    3---2
```

### Initial State

```
parent = [0, 1, 2, 3]
```

### Iteration 1

#### Direct-Connect

- Edge {0,1}: parent[1] = 0
- Edge {1,2}: parent[2] = 1
- Edge {2,3}: parent[3] = 2
- Edge {3,0}: 3 > 0, parent[3] = min(2, 0) = 0

```
parent = [0, 0, 1, 0]

Forest:
     0
    /|\
   0 1 3
     |
     2
```

#### Shortcut

- parent[2] = parent[1] = 0

```
parent = [0, 0, 0, 0]
```

#### Alter

All edges deleted (same parent)

### Final Result

```
parent = [0, 0, 0, 0]
Component 0: {0, 1, 2, 3}
Iterations: 1
```

---

## Complexity Analysis Examples

### Best Case: Star Graph

```
Input: n vertices, n-1 edges (star centered at vertex 0)
Iterations: 1
Time: O(m) = O(n)
```

### Worst Case: Long Path with Specific Structure

```
Input: Path with alternating connections
Iterations: O(log² n)
Time: O(m log² n)
```

### Average Case: Random Graph

```
Input: Random connected graph
Iterations: O(log n)
Time: O(m log n)
```

---

## Parallel Execution Example

### Graph with 6 vertices, 5 edges, 3 threads

```
Edges: {0-1}, {1-2}, {2-3}, {4-5}, {5-6}
```

### Thread Assignment (Direct-Connect)

```
Thread 0: Process edges[0] = {0,1} → parent[1] = 0
Thread 1: Process edges[1] = {1,2} → parent[2] = 1
          Process edges[2] = {2,3} → parent[3] = 2
Thread 2: Process edges[3] = {4,5} → parent[5] = 4
          Process edges[4] = {5,6} → parent[6] = 5
```

All threads execute simultaneously!

### Synchronization Points

1. After Direct-Connect: All threads must finish before Shortcut
2. After Shortcut: All threads must finish before Alter
3. After Alter: Check convergence

### Speedup

```
Serial time: T_s
Parallel time (p threads): T_p ≈ T_s/p + overhead
Speedup: S = T_s/T_p
Efficiency: E = S/p × 100%
```

---

## Key Observations

### 1. Monotonicity

- Parent values only decrease (minimum labeling)
- No cycles created (except self-loops)

### 2. Convergence

- Each iteration reduces tree depth
- Alter step removes intra-component edges
- Terminates when all trees are flat and one per component

### 3. Parallelization

- **Direct-Connect**: Each edge processed independently
- **Shortcut**: Read phase parallel, write phase with reduction
- **Alter**: Each edge updated independently

### 4. Edge Reduction

- Alter step progressively reduces edge count
- Final state: 0 edges (or edges between different components)
- Memory efficiency improves during execution

---

## Comparison with Other Algorithms

### DFS/BFS (Sequential)

```
Time: O(m + n)
Space: O(n)
Parallelization: Difficult
```

### Union-Find (Sequential)

```
Time: O(m α(n)) where α is inverse Ackermann
Space: O(n)
Parallelization: Challenging
```

### Algorithm B (This Implementation)

```
Time: O(m log² n) serial, O((m log² n)/p) parallel
Space: O(m + n)
Parallelization: Natural and efficient
```

---

## Implementation Tips

### 1. Edge Storage

- Use vector for dynamic edge list
- Alter step modifies edge count
- Reserve capacity for efficiency

### 2. Parent Updates

- Use atomic operations in parallel
- Minimum combining for CRCW PRAM
- Reduction for changed flag

### 3. Convergence Check

- Track if any parent changed
- Use reduction (OR) in parallel
- Stop when no changes occur

### 4. Performance Optimization

- Minimize synchronization points
- Balance workload across threads
- Consider cache locality

---

_This document provides visual and conceptual understanding of Algorithm B for the CSS311 assignment._
