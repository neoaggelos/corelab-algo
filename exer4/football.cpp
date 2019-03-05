#include <bits/stdc++.h>

using namespace std;

class Graph {
private:
    int V;
    vector<int> *edges;
    vector<int> *redges;

    // largely based on SCCutil() from tarjan algorithm
    void slave(int u, int disc[], int low[], stack<int> *st, bool in_stack[]) {
        static int time = 0;

        disc[u] = low[u] = ++time;
        st->push(u);
        in_stack[u] = true;

        for (auto it = edges[u].begin(); it != edges[u].end(); it++) {
            if (disc[*it] == -1) {
                slave(*it, disc, low, st, in_stack);
                low[u] = min(low[u], low[*it]);
            } else if (in_stack[*it]) {
                low[u] = min(low[u], disc[*it]);
            }
        }

        if (low[u] == disc[u]) {
            // DISCUSSION
            // this is the part where tarjan algorithm has found a new scc
            // * the vertices of the graph are (u, v) --> u loses to v.
            // * we consider the reverse graph, with edges (u, v) --> u wins v
            //      - for this graph, we are able to create a schedule of
            //        matches so that a particular team wins the whole thing
            //        when every other node is accessibe from this team's node.
            //      - when thinking in terms of strongly connected components,
            //        the only nodes for which this is possible are the ones
            //        that belong in the (topologically) first SCC of the graph
            //      - tarjan algorithm finds SCCs in reverse topological order
            //          ==> for the reverse graph, we would want to fully run
            //              tarjan, find all SCCs and calculate the length of
            //              the last one (which is topologically first)
            // * our graph has the reverse edges of the above. this means that
            //   it has the exact same set of SCCs, in reverse topological order
            //      - for our graph, tarjan will find the same set of SCCs in
            //        reverse order.
            //      - it will find the SCC we are intererested in first
            //
            // CONCLUSION
            // we don't have to run the tarjan algorithm in full, we may stop
            // once we have found the first SCC. print out its length and exit
            int len = 0;

            // wrong results for disconnected graphs without this
            if (!canReachAll(u)) {
                cout << 0 << endl;
                exit(0);
            }

            int w;
            do {
                // normally, we would also do `in_stack[w] = false;`
                // not needed, since we will exit afterwards anyway
                // (only saves a couple of msecs for big N, but oh well)
                w = st->top(); st->pop(); len++;
            } while(w != u);

            cout << len << endl;
            exit(0); // finish after first scc is found
        }
    }

    // check if node u can access all other nodes with reverse edges
    // protects from disconnected graphs doing weird things
    // uses standard DFS
    bool canReachAll(int u) {
        bool *reach = new bool[V];
        for (int i = 0; i < V; i++) reach[i] = false;

        _helper(u, reach);

        for (int i = 0; i < V; i++)
            if (!reach[i]) return false;

        // all reachable
        return true;
    }

    void _helper(int u, bool reach[]) {
        reach[u] = true;
        for (auto it = redges[u].begin(); it != redges[u].end(); it++) {
            if (!reach[*it]) _helper(*it, reach);
        }
    }

public:
    Graph(int n) {
        V = n;
        edges = new vector<int>[V]();
        redges = new vector<int>[V]();
    }

    void addEdge(int u, int w) {
        edges[u].push_back(w);
        redges[w].push_back(u);
    }

    void printResult() {
        int *disc = new int[V];
        int *low = new int[V];
        bool *in_stack = new bool[V];
        stack<int> *st = new stack<int>();
        for(int i = 0; i < V; i++) {
            disc[i] = low[i] = -1;
            in_stack[i] = false;
        }

        // we only want the (topologically) last SCC, see comments above
        // no need to run for the whole graph
        // still, run for all nodes of the graph, so that SCCs are found properly
        for (int i = 0; i < V; i++)
            slave(i, disc, low, st, in_stack);
    }
};

int main(int argc, char** argv)
{
    int N, M, u;
    cin >> N;
    Graph g(N);
    for (int i = 0; i < N; i++) {
        cin >> M;
        for (int j = 0; j < M; j++) {
            cin >> u;
            g.addEdge(i, u-1); // i loses to u
        }
    }

    g.printResult();
}