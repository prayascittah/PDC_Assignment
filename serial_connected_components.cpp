/**
 * Serial Connected Components Algorithm (Algorithm B)
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
        cout << "Edges: ";
        for (const auto& e : edges) {
            cout << "{" << e.src << "," << e.dest << "} ";
        }
        cout << "\n";
    }
};

// Direct-Connect operation
// For each edge {v,w}, update parent of larger vertex to smaller one
bool directConnect(vector<int>& parent, vector<Edge>& edges) {
    bool changed = false;
    
    for (auto& edge : edges) {
        int v = edge.src;
        int w = edge.dest;
        
        if (v > w) {
            int minVal = min(parent[v], w);
            if (parent[v] != minVal) {
                parent[v] = minVal;
                changed = true;
            }
        } else {
            int minVal = min(parent[w], v);
            if (parent[w] != minVal) {
                parent[w] = minVal;
                changed = true;
            }
        }
    }
    
    return changed;
}

// Shortcut operation
// Set parent[v] = parent[parent[v]] for all vertices
bool shortcut(vector<int>& parent) {
    bool changed = false;
    vector<int> oldParent = parent;
    
    for (int i = 0; i < parent.size(); i++) {
        int newParent = oldParent[oldParent[i]];
        if (parent[i] != newParent) {
            parent[i] = newParent;
            changed = true;
        }
    }
    
    return changed;
}

// Alter operation
// Update edges based on current parent values
void alter(vector<int>& parent, vector<Edge>& edges) {
    vector<Edge> newEdges;
    
    for (auto& edge : edges) {
        int v = edge.src;
        int w = edge.dest;
        int pv = parent[v];
        int pw = parent[w];
        
        // Only keep edge if parents are different
        if (pv != pw) {
            // Replace edge with {parent[v], parent[w]}
            newEdges.push_back(Edge(pv, pw));
        }
    }
    
    edges = newEdges;
}

// Main Algorithm B
vector<int> findConnectedComponents(Graph& g, int& iterations) {
    int n = g.numVertices;
    vector<int> parent(n);
    
    // Initialization: each vertex is its own parent
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
    
    iterations = 0;
    bool changed = true;
    
    // Main loop
    while (changed) {
        iterations++;
        changed = false;
        
        // Direct-Connect step
        bool connect_changed = directConnect(parent, g.edges);
        changed = changed || connect_changed;
        
        // Shortcut step
        bool shortcut_changed = shortcut(parent);
        changed = changed || shortcut_changed;
        
        // Alter step
        alter(parent, g.edges);
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
    for (const auto& comp : components) {
        cout << "Component " << comp.first << ": { ";
        for (int v : comp.second) {
            cout << v << " ";
        }
        cout << "}\n";
    }
}

// Test case 1: Simple linear graph
void testCase1() {
    cout << "\n========== TEST CASE 1: Linear Graph ==========\n";
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    
    g.printGraph();
    
    int iterations = 0;
    auto start = high_resolution_clock::now();
    vector<int> parent = findConnectedComponents(g, iterations);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    
    printComponents(parent, g.numVertices);
    cout << "Iterations: " << iterations << "\n";
    cout << "Time: " << duration.count() / 1000.0 << " ms\n";
}

// Test case 2: Two separate components
void testCase2() {
    cout << "\n========== TEST CASE 2: Two Components ==========\n";
    Graph g(10);
    
    // Component 1: vertices 0-4
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    
    // Component 2: vertices 5-9
    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 8);
    g.addEdge(8, 9);
    
    g.printGraph();
    
    int iterations = 0;
    auto start = high_resolution_clock::now();
    vector<int> parent = findConnectedComponents(g, iterations);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    
    printComponents(parent, g.numVertices);
    cout << "Iterations: " << iterations << "\n";
    cout << "Time: " << duration.count() / 1000.0 << " ms\n";
}

// Test case 3: Star graph
void testCase3() {
    cout << "\n========== TEST CASE 3: Star Graph ==========\n";
    Graph g(6);
    
    // Center vertex is 0, connected to all others
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    g.addEdge(0, 5);
    
    g.printGraph();
    
    int iterations = 0;
    auto start = high_resolution_clock::now();
    vector<int> parent = findConnectedComponents(g, iterations);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    
    printComponents(parent, g.numVertices);
    cout << "Iterations: " << iterations << "\n";
    cout << "Time: " << duration.count() / 1000.0 << " ms\n";
}

// Test case 4: Larger graph with multiple components
void testCase4() {
    cout << "\n========== TEST CASE 4: Large Graph with Multiple Components ==========\n";
    Graph g(20);
    
    // Component 1: Complete graph on vertices 0-4
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            g.addEdge(i, j);
        }
    }
    
    // Component 2: Cycle on vertices 5-9
    for (int i = 5; i < 9; i++) {
        g.addEdge(i, i + 1);
    }
    g.addEdge(9, 5);
    
    // Component 3: Path on vertices 10-14
    for (int i = 10; i < 14; i++) {
        g.addEdge(i, i + 1);
    }
    
    // Component 4: Star on vertices 15-19
    for (int i = 16; i < 20; i++) {
        g.addEdge(15, i);
    }
    
    g.printGraph();
    
    int iterations = 0;
    auto start = high_resolution_clock::now();
    vector<int> parent = findConnectedComponents(g, iterations);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    
    printComponents(parent, g.numVertices);
    cout << "Iterations: " << iterations << "\n";
    cout << "Time: " << duration.count() / 1000.0 << " ms\n";
}

// Test case 5: Scalability test - large random graph
void testCase5() {
    cout << "\n========== TEST CASE 5: Scalability Test (Large Graph) ==========\n";
    int n = 1000;
    Graph g(n);
    
    // Create a graph with several components
    // Component 1: vertices 0-299
    for (int i = 0; i < 299; i++) {
        if (i % 10 < 5) {
            g.addEdge(i, i + 1);
        }
        if (i % 20 == 0 && i + 10 < 300) {
            g.addEdge(i, i + 10);
        }
    }
    
    // Component 2: vertices 300-599
    for (int i = 300; i < 599; i++) {
        if (i % 15 < 7) {
            g.addEdge(i, i + 1);
        }
    }
    
    // Component 3: vertices 600-999
    for (int i = 600; i < 999; i++) {
        if (i % 25 < 12) {
            g.addEdge(i, i + 1);
        }
    }
    
    g.printGraph();
    
    int iterations = 0;
    auto start = high_resolution_clock::now();
    vector<int> parent = findConnectedComponents(g, iterations);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    
    // Count components
    set<int> uniqueComponents(parent.begin(), parent.end());
    
    cout << "\nTotal Components: " << uniqueComponents.size() << "\n";
    cout << "Iterations: " << iterations << "\n";
    cout << "Time: " << duration.count() / 1000.0 << " ms\n";
}

int main() {
    cout << "=================================================\n";
    cout << "   Serial Connected Components Algorithm B\n";
    cout << "   Based on Liu & Tarjan (2022)\n";
    cout << "=================================================\n";
    
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    
    cout << "\n=================================================\n";
    cout << "   All tests completed successfully!\n";
    cout << "=================================================\n";
    
    return 0;
}

