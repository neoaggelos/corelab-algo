#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <set>

using namespace std;

typedef map<int, int> ppmap;
typedef set<int> pset;

#define MAXN 180
#define MAXM 180
#define MAXK 100
#define WEIRD 1000000103

unsigned long long T[MAXN+1][MAXM+1][MAXK+1];

template<class T>
bool
contains(T container, int val) {
    return container.find(val) != container.end();
}

int main()
{
    int N, M, K, X;

    // read
    cin >> N >> M >> K >> X;

    // read passages
    ppmap passages;
    pset start_points, end_points;
    for (int i = 0; i < K; i++) {
        int E, S;
        cin >> S >> E;
        passages[E] = S;
        start_points.insert(S);
        end_points.insert(E);
    }

    // init
    for (int x = 0; x <= X; x++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                T[i][j][x] = 0;
            }
        }
        T[N-1][M-1][x] = 1;
    }

    // dynamic programming
    for (int x = 0; x <= X; x++) {
        for (int i = N-1; i >= 0; i--) {
            for (int j = M-1; j >= 0; j--) {
                if (i == N-1 && j == M-1)
                    continue;

                unsigned long long base = 0;
                int point = i * M + j;

                // from passage
                if (contains(end_points, point) && x > 0) {
                    int start = passages[point];
                    base += T[start/M][start%M][x-1] % WEIRD;
                }

                // from the right
                if (j < M-1) {
                    int right_point = point + 1;
                    if (!contains(start_points, right_point))
                        base += T[i][j+1][x] % WEIRD;
                }

                // from below
                if (i < N-1) {
                    int below_point = point + M;
                    if (!contains(start_points, below_point))
                        base += T[i+1][j][x] % WEIRD;
                }

                T[i][j][x] = base % WEIRD;
            }
        }
    }

    cout << T[0][0][X] % WEIRD << endl;
}