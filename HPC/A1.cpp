#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    // 🔵 Parallel BFS
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        vector<int> curr{start}, next;

        visited[start] = true;

        cout << "\nBFS: ";

        while (!curr.empty()) {

            #pragma omp parallel for
            for (int i = 0; i < curr.size(); i++) {
                int node = curr[i];

                #pragma omp critical
                cout << node << " ";

                for (int nbr : adj[node]) {

                    #pragma omp critical
                    {
                        if (!visited[nbr]) {
                            visited[nbr] = true;
                            next.push_back(nbr);
                        }
                    }
                }
            }

            curr = next;
            next.clear();
        }

        cout << endl;
    }

    // 🔴 Parallel DFS Utility
    void dfsUtil(int node, vector<bool>& visited) {

        bool alreadyVisited = false;

        #pragma omp critical
        {
            if (visited[node])
                alreadyVisited = true;
            else {
                visited[node] = true;
                cout << node << " ";
            }
        }

        if (alreadyVisited)
            return;

        for (int nbr : adj[node]) {

            #pragma omp task
            dfsUtil(nbr, visited);
        }

        #pragma omp taskwait
    }

    // 🔴 Parallel DFS
    void parallelDFS(int start) {

        vector<bool> visited(V, false);

        cout << "DFS: ";

        #pragma omp parallel
        {
            #pragma omp single
            dfsUtil(start, visited);
        }

        cout << endl;
    }
};

int main() {

    int V, E, u, v, start;

    cout << "Enter number of vertices: ";
    cin >> V;

    Graph g(V);

    cout << "Enter number of edges: ";
    cin >> E;

    cout << "Enter edges (u v):\n";

    for (int i = 0; i < E; i++) {
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "Enter starting vertex: ";
    cin >> start;

    g.parallelBFS(start);
    g.parallelDFS(start);

    return 0;
}