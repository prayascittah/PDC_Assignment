/**
 * Parallel Connected Components Algorithm (Algorithm B) - OpenMP Version
 * Based on: "Simple Concurrent Connected Components Algorithms"
 * by Liu and Tarjan (2022)
 * 
 * CSS311 - Parallel and Distributed Computing Assignment
 */

#include <bits/stdc++.h>
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

int main() {
    cout << "=================================================\n";
    cout << "  Parallel Connected Components Algorithm B\n";
    cout << "  OpenMP Implementation\n";
    cout << "  Based on Liu & Tarjan (2022)\n";
    cout << "=================================================\n\n";
    
    int numVertices, numEdges;
    
    // Input number of vertices
    cout << "Enter the number of vertices: ";
    cin >> numVertices;
    
    // Validate input
    if (numVertices <= 0) {
        cout << "Error: Number of vertices must be positive!\n";
        return 1;
    }
    
    // Input number of edges
    cout << "Enter the number of edges: ";
    cin >> numEdges;
    
    // Validate input
    if (numEdges < 0) {
        cout << "Error: Number of edges cannot be negative!\n";
        return 1;
    }
    
    // Create graph
    Graph g(numVertices);
    
    cout << "\nEnter the edges (format: vertex1 vertex2):\n";
    cout << "Note: Vertices should be numbered from 0 to " << (numVertices - 1) << "\n\n";
    
    // Input edges
    for (int i = 0; i < numEdges; i++) {
        int u, v;
        cout << "Edge " << (i + 1) << ": ";
        cin >> u >> v;
        
        // Validate edge input
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            cout << "Error: Vertex numbers must be between 0 and " << (numVertices - 1) << "!\n";
            cout << "Please enter this edge again.\n";
            i--;
            continue;
        }
        
        g.addEdge(u, v);
    }
    
    // Get thread configuration
    int maxThreads = omp_get_max_threads();
    cout << "\n=================================================\n";
    cout << "   Thread Configuration\n";
    cout << "=================================================\n";
    cout << "System has " << maxThreads << " threads available\n";
    
    int numThreads;
    cout << "Enter number of threads to use (1-" << maxThreads << "): ";
    cin >> numThreads;
    
    if (numThreads <= 0 || numThreads > maxThreads) {
        cout << "Invalid thread count. Using default: " << min(8, maxThreads) << " threads\n";
        numThreads = min(8, maxThreads);
    }
    
    cout << "\n=================================================\n";
    cout << "   Processing Graph...\n";
    cout << "=================================================\n\n";
    
    g.printGraph();
    
    // Create copies for serial and parallel versions
    Graph gSerial = g;
    Graph gParallel = g;
    
    // ===== SERIAL VERSION =====
    cout << "\n--- Running SERIAL Version ---\n";
    int serialIterations = 0;
    auto startSerial = high_resolution_clock::now();
    
    // Simple serial implementation
    vector<int> parentSerial(numVertices);
    for (int i = 0; i < numVertices; i++) {
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
    
    cout << "Serial execution completed!\n";
    cout << "  Iterations: " << serialIterations << "\n";
    cout << "  Time: " << durationSerial.count() / 1000.0 << " ms\n";
    
    // ===== PARALLEL VERSION =====
    cout << "\n--- Running PARALLEL Version (" << numThreads << " threads) ---\n";
    int parallelIterations = 0;
    auto startParallel = high_resolution_clock::now();
    vector<int> parentParallel = findConnectedComponentsParallel(gParallel, parallelIterations, numThreads);
    auto endParallel = high_resolution_clock::now();
    auto durationParallel = duration_cast<microseconds>(endParallel - startParallel);
    
    cout << "Parallel execution completed!\n";
    cout << "  Iterations: " << parallelIterations << "\n";
    cout << "  Time: " << durationParallel.count() / 1000.0 << " ms\n";
    
    // Display results
    printComponents(parentParallel, numVertices);
    
    // Performance comparison
    cout << "\n=================================================\n";
    cout << "   Performance Comparison\n";
    cout << "=================================================\n";
    
    double speedup = (double)durationSerial.count() / durationParallel.count();
    double efficiency = speedup / numThreads * 100;
    
    cout << "Serial Time:    " << durationSerial.count() / 1000.0 << " ms\n";
    cout << "Parallel Time:  " << durationParallel.count() / 1000.0 << " ms\n";
    cout << "Speedup:        " << speedup << "x\n";
    cout << "Efficiency:     " << efficiency << "%\n";
    cout << "Threads Used:   " << numThreads << "\n";
    
    cout << "\n=================================================\n";
    cout << "   Algorithm completed successfully!\n";
    cout << "=================================================\n";
    
    return 0;
}

