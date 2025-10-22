// Name: Jayanth P
// Roll Number: 2023BCS0040
#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

// Edge structure
struct Edge {
    int src, dest;
    Edge(int s, int d) : src(s), dest(d) {}
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
        } 
        else {
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

void alterParallel(vector<int>& parent, vector<Edge>& edges, int numThreads) {
    vector<bool> toDelete(edges.size(), false);
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < edges.size(); i++) {
        int v = edges[i].src, w = edges[i].dest;
        int pv = parent[v], pw = parent[w];
        if (pv == pw) toDelete[i] = true;
        else {
            edges[i].src = pv;
            edges[i].dest = pw;
        }
    }
    vector<Edge> newEdges;
    for (int i = 0; i < edges.size(); i++) {
        if (!toDelete[i]) {
            newEdges.push_back(edges[i]);
        }
    }
    edges = newEdges;
}

// Main Parallel Algorithm B
vector<int> findConnectedComponentsParallel(Graph& g, int& iterations, int numThreads) {
    int n = g.numVertices;
    vector<int> parent(n);
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
    iterations = 0;
    bool changed = true;
    while (changed) {
        iterations++;
        changed = false;
        bool connect_changed = directConnectParallel(parent, g.edges, numThreads);
        changed = changed || connect_changed;
        bool shortcut_changed = shortcutParallel(parent, numThreads);
        changed = changed || shortcut_changed;
        alterParallel(parent, g.edges, numThreads);
    }
    
    return parent;
}

// Helper function to print components
void printComponents(const vector<int>& parent, int n) {
    cout << "Vertex : Parent (Component)\n";
    for (int i = 0; i < n; i++) {
        cout << i << " : " << parent[i] << "\n";
    }
}

int main() {
    cout << "Name: Jayanth P\n";
    cout << "Roll Number: 2023BCS0040\n";
    int numVertices, numEdges;
    cin >> numVertices >> numEdges;
    Graph g(numVertices);
    for (int i = 0; i < numEdges; i++) {
        int u, v; cin >> u >> v;
        g.addEdge(u, v);
    }
    int maxThreads = omp_get_max_threads();
    int numThreads; cin >> numThreads;
    Graph gParallel = g;
    int parallelIterations = 0;
    double startParallel = omp_get_wtime();
    vector<int> parentParallel = findConnectedComponentsParallel(gParallel, parallelIterations, numThreads);
    double endParallel = omp_get_wtime();
    double durationParallel = (endParallel - startParallel) * 1000.0;
    cout << "Iterations: " << parallelIterations << "\n";
    cout << "Execution Time: " << durationParallel << " ms\n";
    printComponents(parentParallel, numVertices);
    return 0;
}

