
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Function to check if a component is bipartite and return its size
// Returns pair of (is_bipartite, component_size)
pair<bool, int> isBipartite(const vector<vector<int>>& graph, vector<bool>& visited, int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    
    // Color array: -1 = uncolored, 0 = color 0, 1 = color 1
    vector<int> color(graph.size(), -1);
    color[start] = 0;
    
    int componentSize = 1;
    bool isBipartite = true;
    
    while (!q.empty() && isBipartite) {
        int u = q.front();
        q.pop();
        
        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                color[v] = color[u] ^ 1;
                q.push(v);
                componentSize++;
            } else {
                // Check if coloring is consistent
                if (color[v] == color[u]) {
                    isBipartite = false;
                }
            }
        }
    }
    
    return {isBipartite, componentSize};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    // Create adjacency list for the graph
    // Graph has n+1 vertices (1-indexed)
    vector<vector<int>> graph(n + 1);
    
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }
    
    vector<bool> visited(n + 1, false);
    int totalBipartiteVertices = 0;
    
    // Check each connected component
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            auto [isBipartiteComp, compSize] = isBipartite(graph, visited, i);
            if (isBipartiteComp) {
                totalBipartiteVertices += compSize;
            }
        }
    }
    
    cout << totalBipartiteVertices << endl;
    
    return 0;
}
