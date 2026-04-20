#include<bits/stdc++.h>
using namespace std;

int timer_count = 1; // Renamed to avoid conflicts with standard library <time.h>

void dfs(vector<vector<int>>& graph, vector<int>& v, vector<int>& disc, vector<int>& low, set<pair<int,int>>& critical_links, int node, int parent){
    v[node] = 1;
    disc[node] = low[node] = timer_count;
    timer_count++;
    
    // CHANGE 1: Removed 'int children = 0;'
    // Why: The concept of counting independent children only applies to 
    // finding Articulation Points (specifically, the root node rule). 
    // Bridges do not care if a node is the starting root or not.

    for(int nxt : graph[node]){
        if(nxt == parent){
            continue;
        }

        if(!v[nxt]){
            // CHANGE 2: Removed 'children++;' (See CHANGE 1)
            
            dfs(graph, v, disc, low, critical_links, nxt, node);
            low[node] = min(low[node], low[nxt]);
            
            // CHANGE 3: The core condition changed from '>=' to strictly '>'
            // We also completely removed the 'parent != -1' check.
            // 
            // Articulation Point: low[nxt] >= disc[node] 
            // (If child can't reach ABOVE the current node, the node is a bottleneck).
            //
            // Bridge: low[nxt] > disc[node] 
            // (If the child can't reach ABOVE *OR EXACTLY TO* the current node, the edge is a bridge).
            if(low[nxt] > disc[node]){
                
                // CHANGE 4: Storing an Edge instead of a Vertex
                // APs store single integers (the node). Bridges store pairs (the link).
                // We use min() and max() so the edge is always stored in the same order 
                // (e.g., 1-2 instead of 2-1) to avoid duplicates in the set.
                critical_links.insert({min(node, nxt), max(node, nxt)});
            }
        }
        else{
            low[node] = min(low[node], disc[nxt]);
        }
    }
    
    // CHANGE 5: Removed the entire Root Node Check block
    // Why: We deleted 'if(parent == -1 && children > 1) { ... }' completely.
    // If an edge connected to the root is a bridge, the 'low[nxt] > disc[node]' 
    // logic inside the loop will naturally catch it. There are no special cases 
    // for the root when dealing with edges.
}

int main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int vertices;

    // Read until EOF
    while (cin >> vertices) {
        vector<vector<int>> graph(vertices);
        
        for (int i = 0; i < vertices; i++) {
            int u, num_edges;
            char bracket1, bracket2;
            
            // cin gracefully ignores all spaces and newlines
            // Matches format: u (num_edges)
            cin >> u >> bracket1 >> num_edges >> bracket2; 
            
            for (int j = 0; j < num_edges; j++) {
                int v;
                cin >> v;
                // Add the edge only once to avoid duplication in an undirected graph
                if (u < v) {
                    graph[u].push_back(v);
                    graph[v].push_back(u);
                }
            }
        }

        vector<int> visited(vertices, 0);
        vector<int> disc(vertices, -1);
        vector<int> low(vertices, -1);
        set<pair<int,int>> critical_links;

        timer_count = 1;

        // Loop through all vertices to handle disconnected sub-networks
        for (int i = 0; i < vertices; i++) {
            if (!visited[i]) {
                dfs(graph, visited, disc, low, critical_links, i, -1);
            }
        }

        // Output results (std::set automatically sorts the pairs)
        cout << critical_links.size() << " critical links\n";
        for (auto link : critical_links) {
            cout << link.first << " - " << link.second << '\n';
        }
        cout << '\n';
    }

    return 0;
}