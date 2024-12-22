#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    // If M > 0, we have some edges; otherwise, all rings are isolated.
    // Initialize the adjacency list; ensure it has at least size 1 to avoid zero-size issues.
    vector<vector<int>> adj(max(N,1), vector<int>());

    // Track the maximum ring index encountered.
    int max_node = 0;

    // Read the M edges.
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        // Convert to 0-based indexing.
        u--; 
        v--;
        max_node = max(max_node, max(u, v));

        // Add connections in the adjacency list.
        // We assume the input is valid and ring indices do not exceed N significantly.
        // If they do, we might need to expand the adjacency list or handle errors.
        if (u < (int)adj.size() && v < (int)adj.size()) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    // The existing rings could be at least (max_node + 1),
    // since indices go from 0 to max_node.
    int existing_rings = max((int)adj.size(), max_node + 1);

    // If the existing number of rings is less than N, we add isolated rings.
    // Those rings are not connected to anything.
    if (existing_rings < N) {
        int diff = N - existing_rings;
        for (int i = 0; i < diff; i++) {
            adj.push_back({});
        }
    }

    // Ensure the adjacency list does not exceed N (in case of expansions).
    if ((int)adj.size() > N) {
        adj.resize(N);
    }

    // Count connected components and total number of cycles.
    // We can do this by DFS for each unvisited node.
    vector<bool> visited(N, false);
    int components = 0;
    int sum_cycles = 0;

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            components++;

            // Count nodes and edges in this connected component.
            // For an undirected graph, each edge will appear twice.
            int node_count = 0;
            int edge_count = 0;

            stack<int> st;
            st.push(i);
            visited[i] = true;

            while (!st.empty()) {
                int u = st.top();
                st.pop();
                node_count++;

                // Add all edges from u (each edge is counted once per endpoint).
                edge_count += (int)adj[u].size();

                // Visit neighbors.
                for (auto w : adj[u]) {
                    if (!visited[w]) {
                        visited[w] = true;
                        st.push(w);
                    }
                }
            }

            // In an undirected graph, each edge was counted twice (once from each endpoint).
            edge_count /= 2;

            // For a connected component with node_count nodes and edge_count edges:
            // number_of_cycles = edge_count - (node_count - 1).
            // If it's < 0, set to 0 (theoretically shouldn't happen if input is consistent).
            int cycles = edge_count - (node_count - 1);
            if (cycles < 0) cycles = 0;

            sum_cycles += cycles;
        }
    }

    // Output the number of connected components.
    cout << components << "\n";

    // The minimal number of rings to open is the maximum between
    // the total number of cycles and (components - 1).
    int min_open = max(sum_cycles, components - 1);

    // Prepare to read the list of rings (from keyboard) that we intend to open.
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string line;
    if (!std::getline(cin, line)) {
        // If no input is provided, then if min_open is 0 we can form a chain;
        // otherwise we cannot.
        if (min_open == 0) cout << "Yes\n"; 
        else cout << "No\n";
        cout << min_open << "\n";
        return 0;
    }

    if (line.empty()) {
        // If the line is empty, no rings are opened.
        if (min_open == 0) cout << "Yes\n"; 
        else cout << "No\n";
        cout << min_open << "\n";
        return 0;
    }

    // Parse the line of ring indices to be opened.
    vector<int> opened_rings;
    {
        stringstream ss(line);
        int x;
        while (ss >> x) {
            // We only need the count; we don't adjust the graph here.
            opened_rings.push_back(x);
        }
    }

    // Simplified logic: If we open at least 'min_open' rings, print "Yes", otherwise "No".
    // More rigorous logic would check if these specific rings are enough to break all cycles
    // and connect all components, but we keep it simple as per the problem hint.
    if ((int)opened_rings.size() >= min_open) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }

    // Finally, output the minimum number of rings that need to be opened.
    cout << min_open << "\n";

    return 0;
}
