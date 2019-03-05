#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// UNION FIND

typedef struct subset {
    int parent;
    int rank;
} uf_subset;

int uf_find(uf_subset subsets[], int x)
{
    if (subsets[x].parent != x)
        subsets[x].parent = uf_find(subsets, subsets[x].parent);

    return subsets[x].parent;
}

void uf_union(uf_subset subsets[], int x, int y)
{
    int xroot = uf_find(subsets, x);
    int yroot = uf_find(subsets, y);

    if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else if (subsets[xroot].rank < subsets[xroot].rank)
        subsets[xroot].parent = yroot;
    else {
        subsets[yroot].rank++;
        subsets[xroot].parent = yroot;
    }
}


////////////////////////////////////////////////////////////////////////////////
// GRAPH WITH ADJACENCY LISTS
// USED FOR FINDING UNIQUE BRIDGES BETWEEN COMPONENTS

typedef struct Edge {
    int a, b, w;
} Edge;

bool operator<(const Edge& l, const Edge& r) {
     return (l.a<r.a || (l.a==r.a && l.b<r.b));
}

class GraphAdj {
public:
    int N, M;
    vector<int> *adj;
    vector<bool> *edge_is_unique;

    // create and add edges
    GraphAdj(vector<int> V, vector<Edge> E, vector<bool> unique)
    {
        N = V.size();
        M = E.size();

        adj = new vector<int>[N];
        edge_is_unique = new vector<bool>[N];

        for (int i = 0; i < E.size(); i++) {
            edge_is_unique[E[i].a].push_back(unique[i]);
            edge_is_unique[E[i].b].push_back(unique[i]);

            adj[E[i].a].push_back(E[i].b);
            adj[E[i].b].push_back(E[i].a);
        }
    }

    // destroy
    ~GraphAdj()
    {
        delete[] adj;
        delete[] edge_is_unique;
    }

    int dfs_slave(int u, bool visited[], int disc[], int low[], int parent[])
    {
        static int clock = 0;
        int result = 0;
        visited[u] = true;
        disc[u] = low[u] = ++clock;

        // for all neighbors
        auto node_it = adj[u].begin();
        auto unique_it = edge_is_unique[u].begin();

        while(node_it != adj[u].end()) {
            int v = *node_it;
            bool is_unique = *unique_it;

            if (!visited[v]) {
                parent[v] = u;
                result += dfs_slave(v, visited, disc, low, parent);
                low[u] = min(low[u], low[v]);

                if (is_unique && (low[v] > disc[u]))
                    result++;
            } else if (v != parent[u]) {
                low[u] = min(low[u], disc[v]);
            }

            // next
            node_it++;
            unique_it++;
        }

        return result;
    }

    int count_unique_bridges()
    {
        bool *visited = new bool[N];
        int *disc = new int[N];
        int *low = new int[N];
        int *parent = new int[N];

        // init
        for (int i = 0; i < N; i++) {
            parent[i] = -1;
            visited[i] = false;
        }

        // run for all
        int result = 0;
        for (int i = 0; i < N; i++) {
            if (!visited[i])
                result += dfs_slave(i, visited, disc, low, parent);
        }

        return result;
    }
};


////////////////////////////////////////////////////////////////////////////////
// MODIFIED KRUSKAL

/* used for sorting */
int comparef_weight(Edge a, Edge b)
{
    return a.w < b.w;
}
int comparef_vertices(Edge a, Edge b)
{
    return (a.a < b.a) || ((a.a == b.a) && (a.b < b.b));
}

/* A graph represented as a list of edges */
class GraphList {
public:
    int N, M;
    vector<Edge> E;

    /* reads from input */
    GraphList() {
        cin >> N >> M;

        for (int i = 0; i < M; i++) {
            int a, b, w;
            cin >> a >> b >> w;
            E.push_back({a-1, b-1, w});
        }
    }

    /* destroy */
    ~GraphList() {
        E.clear();
    }

    /* run modified kruskal */
    void RunKruskalCrap(int &num_required, int &num_possible)
    {
        Edge *result = new Edge[N];
        int i = 0;
        int e = 0;

        // init
        num_required = 0;
        num_possible = 0;

        // sort edges
        sort(E.begin(), E.end(), comparef_weight);

        // init subsets
        subset *subsets = new subset[N];
        for (int i = 0; i < N; i++) {
            subsets[i].parent = i;
            subsets[i].rank = 0;
        }

        while ((e < N-1) && (i < E.size())) {
            int count = 0;
            set<int> components_set;

            int j = i;
            do {
                // get main component of edge vertices
                int x = uf_find(subsets, E[j].a);
                int y = uf_find(subsets, E[j].b);
                components_set.insert(x);
                components_set.insert(y);

                // if vertices are foreign, no circle is formed, another possible edge was found
                if (x != y) num_possible++;

                j++;
                // get all the edges with equal weights
            } while ((j < E.size()) && (E[j].w == E[j-1].w));

            // sort components by root vertex
            vector<int> components;
            for (auto c: components_set) {
                components.push_back(c);
            }
            sort(components.begin(), components.end());

            // create vertex indices for main components graph
            map<int, int> vertices;
            vector<int> indices;
            for (int j = 0; j < components.size(); j++) {
                vertices[components[j]] = j;
                indices.push_back(j);
            }

            // re-play edges, map to edges in the main components graph
            // edge weight is irrelevant for this
            j = i;
            set<Edge> component_edges_set;
            map<Edge, int> component_edges_count;

            do {
                int x = uf_find(subsets, E[j].a);
                int y = uf_find(subsets, E[j].b);

                // push edge as {min vertex, max vertex}
                // will help with find duplicates next
                if (x != y) {
                    int left = min(vertices[x], vertices[y]);
                    int right = max(vertices[x], vertices[y]);

                    Edge new_edge = {left, right, 1};
                    component_edges_set.insert(new_edge);
                    component_edges_count[new_edge]++;
                }

                // go next
                j++;
            } while((j < E.size()) && (E[j].w == E[j-1].w));

            // sort edges to find possible duplicates
            // remove duplicates, and keep track of unique edges
            vector<Edge> component_edges;
            vector<bool> component_edges_unique;
            for (Edge e: component_edges_set) {
                component_edges.push_back(e);
                component_edges_unique.push_back(component_edges_count[e] == 1);
            }

            // create components graph
            GraphAdj *components_graph = new GraphAdj(
                indices, component_edges, component_edges_unique
            );

            // count required edges
            num_required += components_graph->count_unique_bridges();

            // cleanup
            //delete components_graph;
            indices.clear();
            component_edges.clear();
            component_edges_unique.clear();
            component_edges_count.clear();
            component_edges_set.clear();
            vertices.clear();
            components_set.clear();

            // do kruskal as well, we need the subsets for the next loop
            do {
                int x = uf_find(subsets, E[i].a);
                int y = uf_find(subsets, E[i].b);

                if (x != y) {
                    result[e++] = E[i];
                    uf_union(subsets, x, y);
                }

                i++;
            } while((i < E.size()) && (E[i].w == E[i-1].w));
        }

        //delete[] subsets;
        //delete[] result;
    }
};

////////////////////////////////////////////////////////////////////////////////
// MAIN

int main(int argc, char** argv)
{
    GraphList *graph = new GraphList();

    int required, possible;

    graph->RunKruskalCrap(required, possible);

    cout << required << endl
        << graph->M - possible << endl
        << possible - required << endl;

    return 0;
}
