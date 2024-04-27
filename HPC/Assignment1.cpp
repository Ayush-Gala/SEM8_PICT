// #include <iostream>
// #include <vector>
// #include <queue>
// #include <omp.h>
// #include <chrono>

#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

// Define the graph structure (undirected graph)
class Graph
{
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V)
    {
        this->V = V;
        adj.resize(V);
    }

    // Function to add an edge to the graph
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Parallel Breadth First Search
    // Parallel Breadth First Search
    void parallelBFS(int source)
    {
        vector<bool> visited(adj.size(), false);
        queue<int> q;
        visited[source] = true;
        q.push(source);
        
        while (!q.empty())
        {
            int u;
#pragma omp parallel shared(q, visited)
            {
#pragma omp single
                {
                    u = q.front();
                    q.pop();
                    cout << u << " ";
                }
                if (!(adj[u].size() == 0))
                {
#pragma omp for
                    for (int i = 0; i <= adj[u].size() - 1; ++i)
                    {
                        if (!visited[adj[u][i]])
                        {
#pragma omp critical
                            {
                                q.push(adj[u][i]);
                                visited[adj[u][i]] = true;
                            }
                        }
                    }
                }
            }
        }
    }

    // Parallel Depth First Search
    // Parallel Depth First Search
    void parallelDFSUtil(int v, vector<bool> &visited)
    {
        visited[v] = true;
        cout << v << " ";

// Traverse all adjacent vertices
#pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i)
        {
            int u = adj[v][i];
            if (!visited[u])
            {
                parallelDFSUtil(u, visited);
            }
        }
    }

    void parallelDFS(int source)
    {
        vector<bool> visited(V, false);
        parallelDFSUtil(source, visited);
    }
};

int main()
{
    // Create a graph
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    cout << "Parallel Breadth First Search (BFS) starting from vertex 0: ";
    auto start = high_resolution_clock::now();
    g.parallelBFS(0);
    auto stop = high_resolution_clock::now();
    auto duration_bfs = duration_cast<milliseconds>(stop - start);
    cout << endl;

    cout << "Parallel Depth First Search (DFS) starting from vertex 0: ";
    start = high_resolution_clock::now();
    g.parallelDFS(0);
    stop = high_resolution_clock::now();
    auto duration_dfs = duration_cast<milliseconds>(stop - start);

    cout << endl;
    cout << endl;
    cout << "Difference in timing: " << endl;
    cout << "BFS: " << duration_bfs.count() << " milliseconds" << endl
         << "DFS: " << duration_dfs.count() << " milliseconds" << endl;
    return 0;
}
