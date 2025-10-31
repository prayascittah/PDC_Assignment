// Name: Jayanth P
// Roll Number: 2023BCS0040
// MPI Connected Components Algorithm (Algorithm B)

#include <bits/stdc++.h>
#include <mpi.h>
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
        if (u != v) edges.push_back(Edge(u, v));
    }
};

// MPI Direct-Connect operation
bool directConnectMPI(vector<int>& parent, vector<Edge>& edges, int start_idx, int end_idx) {
    bool changed = false;
    for (int i = start_idx; i < end_idx; i++) {
        int v = edges[i].src, w = edges[i].dest;
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

// MPI Shortcut operation
bool shortcutMPI(vector<int>& parent, int start_idx, int end_idx) {
    bool changed = false;
    vector<int> oldParent = parent;
    for (int i = start_idx; i < end_idx; i++) {
        int newParent = oldParent[oldParent[i]];
        if (parent[i] != newParent) {
            parent[i] = newParent;
            changed = true;
        }
    }
    return changed;
}

// MPI Alter operation
void alterMPI(vector<int>& parent, vector<Edge>& edges, vector<Edge>& localNewEdges, int start_idx, int end_idx) {
    localNewEdges.clear();
    for (int i = start_idx; i < end_idx; i++) {
        int v = edges[i].src, w = edges[i].dest;
        int pv = parent[v], pw = parent[w];
        if (pv != pw) localNewEdges.push_back(Edge(pv, pw));
    }
}

// Main MPI Algorithm B
vector<int> findConnectedComponentsMPI(Graph& g, int& iterations, int rank, int size) {
    int n = g.numVertices;
    vector<int> parent(n);
    
    // Initialize parent array
    for (int i = 0; i < n; i++) parent[i] = i;
    
    iterations = 0;
    bool changed = true;
    
    // Calculate work distribution for edges
    int total_edges = g.edges.size();
    int edges_per_proc = total_edges / size;
    int start_edge = rank * edges_per_proc;
    int end_edge = (rank == size - 1) ? total_edges : start_edge + edges_per_proc;
    
    // Calculate work distribution for vertices
    int vertices_per_proc = n / size;
    int start_vertex = rank * vertices_per_proc;
    int end_vertex = (rank == size - 1) ? n : start_vertex + vertices_per_proc;
    
    while (changed) {
        iterations++;
        bool local_changed = false;
        
        // Broadcast current parent array to all processes
        MPI_Bcast(parent.data(), n, MPI_INT, 0, MPI_COMM_WORLD);
        
        // Direct-Connect step (distributed over edges)
        bool connect_changed = false;
        if (total_edges > 0 && start_edge < end_edge) {
            connect_changed = directConnectMPI(parent, g.edges, start_edge, end_edge);
        }
        
        // Gather all parent arrays and merge
        vector<int> all_parents(n * size);
        MPI_Allgather(parent.data(), n, MPI_INT, all_parents.data(), n, MPI_INT, MPI_COMM_WORLD);
        
        // Merge parent arrays (take minimum for each vertex)
        for (int i = 0; i < n; i++) {
            int min_parent = parent[i];
            for (int p = 0; p < size; p++) {
                min_parent = min(min_parent, all_parents[p * n + i]);
            }
            if (parent[i] != min_parent) {
                parent[i] = min_parent;
                connect_changed = true;
            }
        }
        
        // Shortcut step (distributed over vertices)
        bool shortcut_changed = false;
        if (start_vertex < end_vertex) {
            shortcut_changed = shortcutMPI(parent, start_vertex, end_vertex);
        }
        
        // Gather shortcut results
        vector<int> shortcut_results(n * size);
        MPI_Allgather(parent.data(), n, MPI_INT, shortcut_results.data(), n, MPI_INT, MPI_COMM_WORLD);
        
        // Merge shortcut results
        for (int i = 0; i < n; i++) {
            for (int p = 0; p < size; p++) {
                if (i >= p * vertices_per_proc && 
                    (p == size - 1 || i < (p + 1) * vertices_per_proc)) {
                    if (parent[i] != shortcut_results[p * n + i]) {
                        parent[i] = shortcut_results[p * n + i];
                        shortcut_changed = true;
                    }
                    break;
                }
            }
        }
        
        // Alter step (distributed over edges)
        vector<Edge> localNewEdges;
        if (total_edges > 0 && start_edge < end_edge) {
            alterMPI(parent, g.edges, localNewEdges, start_edge, end_edge);
        }
        
        // Gather all new edges
        vector<int> edge_counts(size);
        int local_edge_count = localNewEdges.size();
        MPI_Allgather(&local_edge_count, 1, MPI_INT, edge_counts.data(), 1, MPI_INT, MPI_COMM_WORLD);
        
        // Calculate total new edges and displacements
        int total_new_edges = 0;
        vector<int> displacements(size);
        for (int i = 0; i < size; i++) {
            displacements[i] = total_new_edges;
            total_new_edges += edge_counts[i];
        }
        
        // Prepare local edges for gathering
        vector<int> local_edges_flat;
        for (const auto& edge : localNewEdges) {
            local_edges_flat.push_back(edge.src);
            local_edges_flat.push_back(edge.dest);
        }
        
        // Gather all new edges
        vector<int> all_new_edges(total_new_edges * 2);
        vector<int> edge_counts_doubled(size), displacements_doubled(size);
        for (int i = 0; i < size; i++) {
            edge_counts_doubled[i] = edge_counts[i] * 2;
            displacements_doubled[i] = displacements[i] * 2;
        }
        
        MPI_Allgatherv(local_edges_flat.data(), local_edge_count * 2, MPI_INT,
                       all_new_edges.data(), edge_counts_doubled.data(), 
                       displacements_doubled.data(), MPI_INT, MPI_COMM_WORLD);
        
        // Reconstruct edge list on all processes
        g.edges.clear();
        for (int i = 0; i < total_new_edges; i++) {
            g.edges.push_back(Edge(all_new_edges[i * 2], all_new_edges[i * 2 + 1]));
        }
        
        // Check if any process had changes
        local_changed = connect_changed || shortcut_changed;
        MPI_Allreduce(&local_changed, &changed, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);
    }
    
    return parent;
}

void printComponents(const vector<int>& parent, int n) {
    cout << "Vertex : Parent (Component)\n";
    for (int i = 0; i < n; i++) {
        cout << i << " : " << parent[i] << "\n";
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        cout << "Name: Jayanth P\n";
        cout << "Roll Number: 2023BCS0040\n";
        cout << "MPI Processes: " << size << "\n";
    }
    
    int numVertices, numEdges;
    Graph g(0);
    
    if (rank == 0) {
        // Root process reads input
        cin >> numVertices >> numEdges;
        g = Graph(numVertices);
        for (int i = 0; i < numEdges; i++) {
            int u, v;
            cin >> u >> v;
            g.addEdge(u, v);
        }
    }
    
    // Broadcast graph data to all processes
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numEdges, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank != 0) {
        g = Graph(numVertices);
    }
    
    // Broadcast edge data
    vector<int> edge_data(numEdges * 2);
    if (rank == 0) {
        for (int i = 0; i < numEdges; i++) {
            edge_data[i * 2] = g.edges[i].src;
            edge_data[i * 2 + 1] = g.edges[i].dest;
        }
    }
    MPI_Bcast(edge_data.data(), numEdges * 2, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank != 0) {
        for (int i = 0; i < numEdges; i++) {
            g.addEdge(edge_data[i * 2], edge_data[i * 2 + 1]);
        }
    }
    
    // Run MPI algorithm
    int iterations = 0;
    double start = MPI_Wtime();
    vector<int> parent = findConnectedComponentsMPI(g, iterations, rank, size);
    double end = MPI_Wtime();
    double duration_ms = (end - start) * 1000.0;
    
    if (rank == 0) {
        cout << "Iterations: " << iterations << "\n";
        cout << "Execution Time: " << duration_ms << " ms\n";
        printComponents(parent, numVertices);
    }
    
    MPI_Finalize();
    return 0;
}
