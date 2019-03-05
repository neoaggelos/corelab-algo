#include <bits/stdc++.h>

using namespace std;

#define INF 0x3fffffff
typedef pair<int, int> ipair;
typedef tuple<int, int, int> iiituple;
#define min(x, y) ( (x) < (y) ? (x) : (y) )

class Graph {
    int V;
    int K;

    vector<ipair> * edges;              // edges[from] = [(node, weight), (node2, weight2), ...]
    vector<ipair> * reverse_edges;      // reverse_edges[from] = [(node, weight), ...]

    int *** mindist;                    // mindist[source][dest][paranomies] = X

public:
    Graph(int N, int k) {
        V = N;
        K = k;

        // reserve memory for V vertices, K paranomies
        edges = new vector<ipair>[V]();
        reverse_edges = new vector<ipair>[V]();
        mindist = new int**[V];
        for (int i = 0; i < V; i++) {
            mindist[i] = new int*[V];
            for (int j = 0; j < V; j++) {
                mindist[i][j] = new int[K+1];
                for (int a = 0; a <= K; a++) {
                    mindist[i][j][a] = INF;
                }
            }
        }
    }

    void printEdges() {
        for (int i = 0; i < V; i++) {
            cout << i << ": ";
            for (auto it = edges[i].begin(); it != edges[i].end(); it++) {
                cout << "(" << it->first << "," << it->second << "), ";
            }
            cout << endl;
        }
    }

    void printShortestPaths(int u) {
        // cout << "SHORTEST PATHS FROM " << u+1 << endl;
        for (int i = 0; i < V; i++) {
            cout << i+1 << ": ";
            for(int k = 0; k <= K; k++) {
                cout << mindist[u][i][k] << "(" << k << "), ";
            }
            cout << endl;
        }
    }

    void addEdge(int from, int to, int weight) {
        // add proper edge
        edges[from].push_back(make_pair(to, weight));

        // add reverse edge
        reverse_edges[to].push_back(make_pair(from, weight));
    }

    void findShortestPaths(int source) {
        priority_queue< iiituple, vector<iiituple>, greater<iiituple> > pq;
        bool f[V][V];
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                f[i][j] = false;

        // (distance, node, paranomies)
        pq.push(make_tuple(0, source, 0));
        mindist[source][source][0] = 0;
        // cout << "\n\nDijkstra for node " << source+1 << endl;

        // repeat while priority queue is not empty
        while (!pq.empty()) {
            iiituple current = pq.top();
            pq.pop();

            // (d, u, p)            <--- (insert lame udp joke here)
            int u = get<1>(current), d = get<0>(current), p = get<2>(current);
            if (f[u][p]) continue;

            f[u][p] = true;
            // cout << "DIJKSTRA, distance is " << d << ". standing at node " << u+1 << "(" << p << ")" << endl;

            // normal edges
            for (auto it = edges[u].begin(); it != edges[u].end(); it++) {
                // edge (u, v) with weight w
                int v = it->first, w = it->second;

                if (f[v][p]) continue;
                if (mindist[source][u][p] + w < mindist[source][v][p]) {
                    // found better distance
                    mindist[source][v][p] = mindist[source][u][p] + w;
                    pq.push(make_tuple(mindist[source][v][p], v, p));
                }
            }

            // max paranomies reached, bye
            if (p == K) continue;

            // reverse edges
            for (auto it = reverse_edges[u].begin(); it != reverse_edges[u].end(); it++) {
                // reverse edge (u, v) with weight w
                // same as before, difference is that we must also increase p by 1
                int v = it->first, w = it->second;

                if (f[v][p+1]) continue;
                if (mindist[source][u][p] + w < mindist[source][v][p+1]) {
                    // found better distance
                    mindist[source][v][p+1] = mindist[source][u][p] + w;
                    pq.push(make_tuple(mindist[source][v][p+1], v, p+1));
                }
            }
        }

        // we calculated shortest paths with exactly `p` paranomies
        // we want shortest paths with at most `p` paranomies
        for (int i = 1; i <= K; i++)
            for (int u = 0; u < V; u++) 
                mindist[source][u][i] = min(mindist[source][u][i], mindist[source][u][i-1]);
    }

    void calculateAll() {
        for (int u = 0; u < V; u++) {
            findShortestPaths(u);
            // printShortestPaths(u);
        }
    }

    int minDist(int u, int v, int p) {
        return mindist[u][v][p];
    }
};

int main(int argc, char** argv)
{
    int N, M, K, Q;
    cin >> N >> M >> K >> Q;

    Graph graph(N, K);
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph.addEdge(u-1, v-1, w);
    }
    graph.calculateAll();

    for (int i = 0; i < Q; i++) {
        int u, v, k;
        cin >> u >> v >> k;
        int d = graph.minDist(u-1, v-1, k);

        if (d == INF) {
            cout << "IMPOSSIBLE" << endl;
        } else {
            cout << d << endl;
        }
    }
}