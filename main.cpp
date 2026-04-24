
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

// Global variables for DFS
vector<int> disc;           // Discovery time of vertices
vector<int> low;            // Low value of vertices
vector<bool> visited;       // To track visited vertices
vector<vector<int>> graph;  // Graph adjacency list
stack<pair<int, int>> st;   // Stack to store edges

// Function to perform DFS and find biconnected components
void dfs(int u, int parent, vector<vector<int>>& components) {
    static int time = 0;
    disc[u] = low[u] = ++time;
    visited[u] = true;
    
    int children = 0;
    
    for (int v : graph[u]) {
        if (!visited[v]) {
            children++;
            st.push({u, v});
            
            dfs(v, u, components);
            
            low[u] = min(low[u], low[v]);
            
            // If u is an articulation point or root with more than one child
            if ((parent == -1 && children > 1) || (parent != -1 && low[v] >= disc[u])) {
                // Found a biconnected component
                vector<int> component;
                while (st.top() != make_pair(u, v)) {
                    auto edge = st.top();
                    st.pop();
                    component.push_back(edge.first);
                    component.push_back(edge.second);
                }
                auto edge = st.top();
                st.pop();
                component.push_back(edge.first);
                component.push_back(edge.second);
                
                // Remove duplicates and sort
                sort(component.begin(), component.end());
                component.erase(unique(component.begin(), component.end()), component.end());
                components.push_back(component);
            }
        } else if (v != parent && disc[v] < disc[u]) {
            // Back edge
            low[u] = min(low[u], disc[v]);
            st.push({u, v});
        }
    }
}

// Function to check if a component is bipartite
bool isBipartite(const vector<int>& component, const vector<vector<int>>& graph) {
    // Create a subgraph with only the vertices in the component
    vector<int> color(100001, -1);  // -1: uncolored, 0: color 0, 1: color 1
    vector<bool> inComponent(100001, false);
    
    for (int v : component) {
        inComponent[v] = true;
    }
    
    for (int v : component) {
        if (color[v] == -1) {
            color[v] = 0;
            stack<int> s;
            s.push(v);
            
            while (!s.empty()) {
                int u = s.top();
                s.pop();
                
                for (int w : graph[u]) {
                    if (inComponent[w]) {
                        if (color[w] == -1) {
                            color[w] = color[u] ^ 1;
                            s.push(w);
                        } else if (color[w] == color[u]) {
                            return false;  // Not bipartite
                        }
                    }
                }
            }
        }
    }
    
    return true;  // Bipartite
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    // Resize global vectors
    disc.resize(n + 1);
    low.resize(n + 1);
    visited.resize(n + 1, false);
    graph.resize(n + 1);
    
    // Create adjacency list for the graph
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }
    
    vector<vector<int>> components;
    
    // Find biconnected components
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            dfs(i, -1, components);
        }
    }
    
    // Add remaining edges in the stack
    if (!st.empty()) {
        vector<int> component;
        while (!st.empty()) {
            auto edge = st.top();
            st.pop();
            component.push_back(edge.first);
            component.push_back(edge.second);
        }
        
        // Remove duplicates and sort
        sort(component.begin(), component.end());
        component.erase(unique(component.begin(), component.end()), component.end());
        components.push_back(component);
    }
    
    // Mark vertices that are in non-bipartite biconnected components
    vector<bool> inOddCycle(n + 1, false);
    
    for (const auto& component : components) {
        if (!isBipartite(component, graph)) {
            for (int v : component) {
                inOddCycle[v] = true;
            }
        }
    }
    
    // Count vertices that are not in any odd cycle
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (!inOddCycle[i]) {
            count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
