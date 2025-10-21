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
    vector <Edge> edges;
    Graph(int n) : numVertices(n) {}
    void addEdge(int u, int v) {
        if (u != v) edges.push_back(Edge(u, v));
    }
};

// Direct-Connect operation
bool directConnect(vector<int>& parent, vector<Edge>& edges) {
    bool changed = false;
    for (auto& edge : edges) {
        int v = edge.src, w = edge.dest;
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

void alter(vector<int>& parent, vector<Edge>& edges) {
    vector <Edge> newEdges;
    for (auto& edge : edges) {
        int v = edge.src;
        int w = edge.dest;
        int pv = parent[v];
        int pw = parent[w];
        if (pv != pw) newEdges.push_back(Edge(pv, pw));
    }
    edges = newEdges;
}

// Main Algorithm B
vector<int> findConnectedComponents(Graph& g, int& iterations) {
    int n = g.numVertices;
    vector <int> parent(n);
    for (int i = 0; i < n; i++) parent[i] = i;
    iterations = 0;
    bool changed = true;
    while (changed) {
        iterations++;
        changed = false;
        bool connect_changed = directConnect(parent, g.edges);
        changed = changed || connect_changed;
        bool shortcut_changed = shortcut(parent);
        changed = changed || shortcut_changed;
        alter(parent, g.edges);
    }
    return parent;
}

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
    int iterations = 0;
    double start = omp_get_wtime();
    vector <int> parent = findConnectedComponents(g, iterations);
    double end = omp_get_wtime();
    double duration_ms = (end - start) * 1000.0;

    printComponents(parent, g.numVertices);
    cout << "Iterations: " << iterations << "\n";
    cout << "Execution Time: " << duration_ms << " ms\n";
    return 0;
}

