/**
 * Parallel Connected Components Algorithm (Algorithm B) - OpenMP Version
 * Based on: "Simple Concurrent Connected Components Algorithms"
 * by Liu and Tarjan (2022)
 * 
 * CSS311 - Parallel and Distributed Computing Assignment
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
#include <set>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Edge structure
struct Edge {
    int src, dest;
    
    Edge(int s, int d) : src(s), dest(d) {}
    
    bool operator==(const Edge& other) const {
        return (src == other.src && dest == other.dest) ||
               (src == other.dest && dest == other.src);
    }
};

// Graph class
class Graph {
public:
    int numVertices;
    vector<Edge> edges;
    
    Graph(int n) : numVertices(n) {}
    
    void addEdge(int u, int v) {
        if (u != v) {  // No self-loops
            edges.push_back(Edge(u, v));
        }
    }
    
    void printGraph() {
        cout << "Graph: " << numVertices << " vertices, " 
             << edges.size() << " edges\n";
    }
};

// Parallel Direct-Connect operation with OpenMP
bool directConnectParallel(vector<int>& parent, vector<Edge>& edges, int numThreads) {
    bool changed = false;
    
    #pragma omp parallel for reduction(||:changed) num_threads(numThreads)
    for (int i = 0; i < edges.size(); i++) {
        int v = edges[i].src;
        int w = edges[i].dest;
        
        if (v > w) {
            int minVal = min(parent[v], w);
            if (parent[v] != minVal) {
                #pragma omp atomic write
                parent[v] = minVal;
                changed = true;
            }
        } else {
            int minVal = min(parent[w], v);
            if (parent[w] != minVal) {
                #pragma omp atomic write
                parent[w] = minVal;
                changed = true;
            }
        }
    }
    
    return changed;
}

// Parallel Shortcut operation with OpenMP
bool shortcutParallel(vector<int>& parent, int numThreads) {
    bool changed = false;
    vector<int> oldParent(parent.size());
    
    // Phase 1: Copy current parent values (parallel read)
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < parent.size(); i++) {
        oldParent[i] = parent[i];
    }
    
    // Phase 2: Update parent values (parallel write)
    #pragma omp parallel for reduction(||:changed) num_threads(numThreads)
    for (int i = 0; i < parent.size(); i++) {
        int newParent = oldParent[oldParent[i]];
        if (parent[i] != newParent) {
            parent[i] = newParent;
            changed = true;
        }
    }
    
    return changed;
}

// Parallel Alter operation with OpenMP
void alterParallel(vector<int>& parent, vector<Edge>& edges, int numThreads) {
    vector<bool> toDelete(edges.size(), false);
    
    // Mark edges for deletion and update edge endpoints
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < edges.size(); i++) {
        int v = edges[i].src;
        int w = edges[i].dest;
        int pv = parent[v];
        int pw = parent[w];
        
        if (pv == pw) {
            toDelete[i] = true;
        } else {
            edges[i].src = pv;
            edges[i].dest = pw;
        }
    }
    
    // Compact the edge list (remove deleted edges)
    vector<Edge> newEdges;
    for (int i = 0; i < edges.size(); i++) {
        if (!toDelete[i]) {
            newEdges.push_back(edges[i]);
        }
    }
    edges = newEdges;
}

// Main Parallel Algorithm B with OpenMP
vector<int> findConnectedComponentsParallel(Graph& g, int& iterations, int numThreads) {
    int n = g.numVertices;
    vector<int> parent(n);
    
    // Parallel Initialization
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
    
    iterations = 0;
    bool changed = true;
    
    // Main loop
    while (changed) {
        iterations++;
        changed = false;
        
        // Parallel Direct-Connect step
        bool connect_changed = directConnectParallel(parent, g.edges, numThreads);
        changed = changed || connect_changed;
        
        // Parallel Shortcut step
        bool shortcut_changed = shortcutParallel(parent, numThreads);
        changed = changed || shortcut_changed;
        
        // Parallel Alter step
        alterParallel(parent, g.edges, numThreads);
    }
    
    return parent;
}

// Helper function to print components
void printComponents(const vector<int>& parent, int n) {
    map<int, vector<int>> components;
    
    for (int i = 0; i < n; i++) {
        components[parent[i]].push_back(i);
    }
    
    cout << "\nConnected Components (" << components.size() << " total):\n";
    int count = 0;
    for (const auto& comp : components) {
        cout << "Component " << comp.first << ": { ";
        for (int v : comp.second) {
            cout << v << " ";
        }
        cout << "}\n";
        count++;
        if (count >= 5 && components.size() > 5) {
            cout << "... and " << (components.size() - 5) << " more components\n";
            break;
        }
    }
}

// Compare serial vs parallel performance
void comparePerformance(Graph& g, const string& testName, int numThreads) {
    cout << "\n========== " << testName << " ==========\n";
    g.printGraph();
    
    // Create copies for each version
    Graph gSerial = g;
    Graph gParallel = g;
    
    // Serial version
    int serialIterations = 0;
    auto startSerial = high_resolution_clock::now();
    
    // Simple serial implementation
    vector<int> parentSerial(g.numVertices);
    for (int i = 0; i < g.numVertices; i++) {
        parentSerial[i] = i;
    }
    
    bool changed = true;
    while (changed) {
        serialIterations++;
        changed = false;
        
        // Direct-Connect
        for (auto& edge : gSerial.edges) {
            int v = edge.src;
            int w = edge.dest;
            if (v > w) {
                int minVal = min(parentSerial[v], w);
                if (parentSerial[v] != minVal) {
                    parentSerial[v] = minVal;
                    changed = true;
                }
            } else {
                int minVal = min(parentSerial[w], v);
                if (parentSerial[w] != minVal) {
                    parentSerial[w] = minVal;
                    changed = true;
                }
            }
        }
        
        // Shortcut
        vector<int> oldParent = parentSerial;
        for (int i = 0; i < parentSerial.size(); i++) {
            int newParent = oldParent[oldParent[i]];
            if (parentSerial[i] != newParent) {
                parentSerial[i] = newParent;
                changed = true;
            }
        }
        
        // Alter
        vector<Edge> newEdges;
        for (auto& edge : gSerial.edges) {
            int pv = parentSerial[edge.src];
            int pw = parentSerial[edge.dest];
            if (pv != pw) {
                newEdges.push_back(Edge(pv, pw));
            }
        }
        gSerial.edges = newEdges;
    }
    
    auto endSerial = high_resolution_clock::now();
    auto durationSerial = duration_cast<microseconds>(endSerial - startSerial);
    
    // Parallel version
    int parallelIterations = 0;
    auto startParallel = high_resolution_clock::now();
    vector<int> parentParallel = findConnectedComponentsParallel(gParallel, parallelIterations, numThreads);
    auto endParallel = high_resolution_clock::now();
    auto durationParallel = duration_cast<microseconds>(endParallel - startParallel);
    
    // Print results
    printComponents(parentParallel, g.numVertices);
    
    cout << "\n--- Performance Comparison ---\n";
    cout << "Serial Version:\n";
    cout << "  Iterations: " << serialIterations << "\n";
    cout << "  Time: " << durationSerial.count() / 1000.0 << " ms\n";
    
    cout << "\nParallel Version (" << numThreads << " threads):\n";
    cout << "  Iterations: " << parallelIterations << "\n";
    cout << "  Time: " << durationParallel.count() / 1000.0 << " ms\n";
    
    double speedup = (double)durationSerial.count() / durationParallel.count();
    double efficiency = speedup / numThreads * 100;
    
    cout << "\nSpeedup: " << speedup << "x\n";
    cout << "Efficiency: " << efficiency << "%\n";
}

// Test case 1: Small graph
void testCase1(int numThreads) {
    Graph g(10);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(8, 9);
    
    comparePerformance(g, "TEST CASE 1: Small Graph", numThreads);
}

// Test case 2: Medium graph
void testCase2(int numThreads) {
    Graph g(50);
    
    // Create multiple components
    for (int i = 0; i < 10; i++) {
        g.addEdge(i, i + 1);
    }
    for (int i = 15; i < 25; i++) {
        g.addEdge(i, i + 1);
    }
    for (int i = 30; i < 40; i++) {
        g.addEdge(i, i + 1);
    }
    
    comparePerformance(g, "TEST CASE 2: Medium Graph", numThreads);
}

// Test case 3: Large graph with dense connections
void testCase3(int numThreads) {
    Graph g(100);
    
    // Create a graph with several dense components
    for (int comp = 0; comp < 5; comp++) {
        int start = comp * 20;
        int end = start + 20;
        for (int i = start; i < end - 1; i++) {
            for (int j = i + 1; j < end; j++) {
                if ((i + j) % 3 == 0) {
                    g.addEdge(i, j);
                }
            }
        }
    }
    
    comparePerformance(g, "TEST CASE 3: Large Dense Graph", numThreads);
}

// Test case 4: Very large graph for scalability testing
void testCase4(int numThreads) {
    Graph g(1000);
    
    // Create multiple components with varying connectivity
    for (int i = 0; i < 999; i++) {
        if (i % 50 < 25) {
            g.addEdge(i, i + 1);
        }
        if (i % 100 < 50 && i + 10 < 1000) {
            g.addEdge(i, i + 10);
        }
    }
    
    comparePerformance(g, "TEST CASE 4: Very Large Graph (Scalability Test)", numThreads);
}

// Thread scaling analysis
void threadScalingAnalysis() {
    cout << "\n\n========== THREAD SCALING ANALYSIS ==========\n";
    
    // Create a fixed graph
    Graph g(500);
    for (int i = 0; i < 499; i++) {
        if (i % 25 < 15) {
            g.addEdge(i, i + 1);
        }
        if (i % 50 < 25 && i + 5 < 500) {
            g.addEdge(i, i + 5);
        }
    }
    
    cout << "Graph: " << g.numVertices << " vertices, " << g.edges.size() << " edges\n";
    cout << "\nThread Count | Time (ms) | Speedup | Efficiency\n";
    cout << "-------------|-----------|---------|------------\n";
    
    double baselineTime = 0;
    
    for (int threads : {1, 2, 4, 8, 16}) {
        Graph gCopy = g;
        int iterations = 0;
        
        auto start = high_resolution_clock::now();
        vector<int> parent = findConnectedComponentsParallel(gCopy, iterations, threads);
        auto end = high_resolution_clock::now();
        
        auto duration = duration_cast<microseconds>(end - start);
        double timeMs = duration.count() / 1000.0;
        
        if (threads == 1) {
            baselineTime = timeMs;
        }
        
        double speedup = baselineTime / timeMs;
        double efficiency = speedup / threads * 100;
        
        printf("%12d | %9.3f | %7.2fx | %9.1f%%\n", 
               threads, timeMs, speedup, efficiency);
    }
}

int main() {
    cout << "=================================================\n";
    cout << "  Parallel Connected Components Algorithm B\n";
    cout << "  OpenMP Implementation\n";
    cout << "  Based on Liu & Tarjan (2022)\n";
    cout << "=================================================\n";
    
    // Get number of available threads
    int maxThreads = omp_get_max_threads();
    cout << "\nSystem has " << maxThreads << " threads available\n";
    
    int numThreads = min(8, maxThreads);  // Use up to 8 threads
    cout << "Using " << numThreads << " threads for testing\n";
    
    // Run test cases
    testCase1(numThreads);
    testCase2(numThreads);
    testCase3(numThreads);
    testCase4(numThreads);
    
    // Thread scaling analysis
    threadScalingAnalysis();
    
    cout << "\n=================================================\n";
    cout << "   All tests completed successfully!\n";
    cout << "=================================================\n";
    
    return 0;
}

