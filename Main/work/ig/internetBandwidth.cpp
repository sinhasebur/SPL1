#include <bits/stdc++.h>

using namespace std;

#define ll long long
const ll INF = 1e18;

// Keep your BFS exactly as it was
pair<bool, vector<ll>> bfs(vector<vector<ll>> &g, vector<vector<ll>> &capacity, ll starting_node, ll target_node) {
    ll n = g.size();
    vector<ll> parent(n, -1);
    vector<bool> visited(n, false);
    queue<ll> q;

    q.push(starting_node);
    visited[starting_node] = true;

    while (!q.empty()) {
        ll current_node = q.front();
        q.pop();

        for (auto &nxt : g[current_node]) {
            if (!visited[nxt] && capacity[current_node][nxt] > 0) {
                q.push(nxt);
                visited[nxt] = true;
                parent[nxt] = current_node;
                if (nxt == target_node) return {true, parent};
            }
        }
    }
    return {false, parent};
}

// Keep your Edmonds-Karp exactly as it was
ll edmonds_karp(vector<vector<ll>> &g, vector<vector<ll>> &capacity, ll starting_node, ll target_node) {
    ll max_flow = 0;
    while (true) {
        pair<bool, vector<ll>> bfs_result = bfs(g, capacity, starting_node, target_node);
        if (!bfs_result.first) break;

        vector<ll> parent = bfs_result.second;
        ll path_flow = INF;
        ll current_node = target_node;

        while (current_node != starting_node) {
            ll prev_node = parent[current_node];
            path_flow = min(path_flow, capacity[prev_node][current_node]);
            current_node = prev_node;
        }

        current_node = target_node;
        while (current_node != starting_node) {
            ll prev_node = parent[current_node];
            capacity[prev_node][current_node] -= path_flow;
            capacity[current_node][prev_node] += path_flow;
            current_node = prev_node;
        }
        max_flow += path_flow;
    }
    return max_flow;
}

int main() {
    ll nodes, s, t, connections, network_id = 1;

    // Loop to handle multiple networks until n = 0
    while (cin >> nodes && nodes != 0) {
        cin >> s >> t >> connections;

        // Convert 1-based source and target to 0-based
        s--; t--;

        vector<vector<ll>> graph(nodes);
        vector<vector<ll>> capacity(nodes, vector<ll>(nodes, 0));

        for (int i = 0; i < connections; i++) {
            ll u, v, cap;
            cin >> u >> v >> cap;
            u--; v--; // Convert to 0-based

            // Bi-directional bandwidth: add capacity to both directions
            // If an edge already exists, += aggregates the bandwidth
            if (capacity[u][v] == 0 && capacity[v][u] == 0) {
                graph[u].push_back(v);
                graph[v].push_back(u);
            }
            capacity[u][v] += cap;
            capacity[v][u] += cap;
        }

        ll result = edmonds_karp(graph, capacity, s, t);

        // Required output formatting
        cout << "Network " << network_id++ << endl;
        cout << "The bandwidth is " << result << "." << endl << endl;
    }

    return 0;
}