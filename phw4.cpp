#include <bits/stdc++.h>

using namespace std;

int main() {
    // Prompt for N and M
    cout << "Please input N (total number of rings) and M (number of existing connections), separated by a space: ";

    int N, M;
    cin >> N >> M;

    // Initialize adjacency list with size max(N,1) to avoid zero-size issues
    vector<vector<int>> adj( max(N,1), vector<int>() );

    // Keep track of the maximum ring index encountered
    int max_node = 0;

    // If M > 0, read in the M edges
    if ( M > 0 ) cout << "Now input the M connections (each connection in a separate line, e.g. \"1 2\"):\n";

    for ( int i = 0; i < M; i++ ) {
        int u, v;
        cin >> u >> v;
        // Convert 1-based index to 0-based
        u--;
        v--;

        max_node = max(max_node, max(u, v));

        // Store the edge in the adjacency list (undirected)
        if ( ( u >= 0 ) && ( u < (int)adj.size() ) && ( v >= 0 ) && ( v < (int)adj.size() ) ) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    // Ensure total rings are up to N
    int existing_rings = max( (int)adj.size(), max_node + 1 );

    // If existing rings < N, add isolated rings
    if ( existing_rings < N ) {
        int diff = N - existing_rings;
        for ( int i = 0; i < diff; i++ ) {
            adj.push_back({});
        }
    }

    // If adjacency list is somehow larger than N, shrink it
    if( (int)adj.size() > N ) adj.resize(N);

    // We will count connected components and cycles
    vector<bool> visited( N, false );
    int components = 0;
    int sum_cycles = 0;

    // DFS to find connected components and edges in each component
    for ( int i = 0; i < N; i++ ) {
        if ( !visited[i] ) {
            components++;

            int node_count = 0;
            int edge_count = 0;

            stack<int> st;
            st.push(i);
            visited[i] = true;

            while ( !st.empty() ) {
                int u = st.top();
                st.pop();
                node_count++;

                // Each undirected edge is counted from each endpoint
                edge_count += (int)adj[u].size();

                for ( auto w : adj[u] ) {
                    if ( !visited[w] ) {
                        visited[w] = true;
                        st.push(w);
                    }
                }
            }

            // In undirected graph, each edge is seen twice
            edge_count /= 2;

            // Number of cycles in this component = edge_count - (node_count - 1)
            int cycles = edge_count - (node_count - 1);
            if ( cycles < 0 ) cycles = 0;
            sum_cycles += cycles;
        }
    }

    // Output the number of connected components
    cout << "\nNumber of connected components: " << components << "\n";

    // Minimum number of rings to open
    int min_open = max( sum_cycles, components - 1 );

    // Prompt for rings to open
    cout << "Please input the ring indices you plan to open (space-separated), then press ENTER.\n";
    cout << "(Leave it blank if you do not wish to open any ring.)\n";

    // Clear the stream for reading a line of ring indices
    cin.clear();
    cin.ignore( numeric_limits<streamsize>::max(), '\n' );

    // Read the line
    string line;
    if ( !getline(cin, line) ) {
        // No line read at all
        cout << "\nYou chose to open 0 rings.\n";
        if ( min_open == 0 ) cout << "Yes\n";
		else cout << "No\n";

        cout << "Minimum rings to open (min_open): " << min_open << "\n";


        return 0;
    }

    // If the line is empty, user is opening 0 rings
    if ( line.empty() ) {
        cout << "\nYou chose to open 0 rings.\n";
        if ( min_open == 0 ) cout << "Yes\n";
		else cout << "No\n";

        cout << "Minimum rings to open (min_open): " << min_open << "\n";


        return 0;
    }

    // Parse the ring indices from the line
    vector<int> opened_rings;
    {
        stringstream ss( line );
        int x;
        while ( ss >> x ) {
            opened_rings.push_back(x);
        }
    }

    // Compare how many rings are opened with min_open
    cout << "\nYou chose to open " << opened_rings.size() << " ring(s).\n";
    if ( (int) opened_rings.size() >= min_open ) cout << "Yes\n";
	else cout << "No\n";

    cout << "Minimum rings to open (min_open): " << min_open << "\n";


    return 0;
}
