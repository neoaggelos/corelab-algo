#include <bits/stdc++.h>

using namespace std;

class Graph {
private:
    int V;
    vector<int> *edges;
    int len;

    void slave(int u, int disc[], int low[], stack<int> *st, bool in_stack[]) {
        static int time = 0;

        disc[u] = low[u] = ++time;
        st->push(u);
        in_stack[u] = true;

        // for all reachable
        for (auto it = edges[u].begin(); it != edges[u].end(); it++) {
            if (disc[*it] == -1) {
                slave(*it, disc, low, st, in_stack);
                low[u] = min(low[u], low[*it]);
            } else if (in_stack[*it]) {
                low[u] = min(low[u], disc[*it]);
            }
        }
    
        // found head when finishing recursion
        // print new scc
        if (low[u] == disc[u]) {
            len = 0;
            // cout << "another: ";

            int w;
            do {
                w = st->top();
                in_stack[w] = false;
                st->pop();
                len++;
                // cout << w+1 << " ";
            } while(w != u);
            // cout << endl;
        }
    }

public:
    Graph(int n) {
        V = n;
        edges = new vector<int>[V]();
        len = 0;
    }

    void addEdge(int u, int w) {
        edges[u].push_back(w);
    }

    void printNeighbors() {
        for (int i = 0; i < V; i++) {
            cout << i+1 << ": ";
            for (auto it = edges[i].begin(); it != edges[i].end(); it++) {
                int v = *it;
                cout << v+1 << " ";
            }
            cout << endl;
        }
    }

    // IDEA
    // (u, v) means "u wins v", so winners can be found in the (topologically) first scc of our graph
    // if we had (u, v) "u loses to v", the winners would be in the last scc
    // tarjan finds scc's in reverse order, we could save a lot of time?
    int work() {
        int *disc = new int[V];
        int *low = new int[V];
        bool *in_stack = new bool[V];
        stack<int> *st = new stack<int>();
        for(int i = 0; i < V; i++) {
            disc[i] = low[i] = -1;
            in_stack[i] = false;
        }

        for (int i = 0; i < V; i++) {
            if (disc[i] == -1) {
                slave(i, disc, low, st, in_stack);
            }
        }

        // FIXME: not done
        // this is wrong for certain topologies
        // we need to make sure that all nodes are reachable from final node in the reverse graph
        return len;
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
            g.addEdge(u-1, i); // i loses to u => u wins i
        }
    }
    // cout << "read" << endl;
    // g.printNeighbors();

    cout << g.work() << endl;
}