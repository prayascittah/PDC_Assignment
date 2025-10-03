#include <bits/stdc++.h>
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
    vector <Edge> edges;
    
    Graph(int n) : numVertices(n) {}
    
    void addEdge(int u, int v) {
        if (u != v) {  // No self-loops
            edges.push_back(Edge(u, v));
        }
    }
    
    void printGraph() {
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

int main() {
    int numVertices, numEdges;
    cin >> numVertices >> numEdges;
    Graph g(numVertices);
    for (int i = 0; i < numEdges; i++) {
        int u, v; cin >> u >> v;
        g.addEdge(u, v);
    }
    g.printGraph();
    int iterations = 0;
    auto start = high_resolution_clock::now();
    vector<int> parent = findConnectedComponents(g, iterations);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    
    // Display results
    printComponents(parent, g.numVertices);
    cout << "Iterations: " << iterations << "\n";
    cout << "Execution Time: " << duration.count() / 1000.0 << " ms\n";
    cout << "Time Complexity: O(m log² n) where m = " << numEdges 
         << ", n = " << numVertices << "\n";
    
    
    return 0;
}

