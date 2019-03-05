#include <iostream>
#include <vector>

using namespace std;

#define max(a, b) ((a) > (b) ? (a) : (b))

int main(int argc, char** argv) {
    long long **A_with_cheat, **A;
    long long *X;

    /** INPUT **/
    int N;
    cin >> N;

    X = new long long[N];
    for (int i = 0; i < N; i++) {
        cin >> X[i];
    }

    /** INIT **/
    A_with_cheat = new long long*[N];
    A = new long long*[N];

    for (int i = 0; i < N; i++) {
        A[i] = new long long[N+1];
        A_with_cheat[i] = new long long[N+1];
    }

    /** DP **/
    for (int i = N-1; i >= 0; i--) {
        A[i][N] = 1;
        A_with_cheat[i][N] = 1;
        for (int j = N-1; j > i; j--) {
            if (X[i] >= X[j]) {
                /* we need to cheat if we are to take this one into account */
                A[i][j] = A[i][j+1];
                A_with_cheat[i][j] = max(A_with_cheat[i][j+1], 1 + A[j][j+1]);
            } else {
                /* no need to cheat, keep going */
                A[i][j] = max(A[i][j+1], 1 + A[j][j+1]);
                A_with_cheat[i][j] = max(A_with_cheat[i][j+1], 1 + A_with_cheat[j][j+1]);
            }
        }
    }

    // for (int j = 0; j < N+1; j++) {
    //     cout << X[j] << "\t\t";
    // }
    // cout << endl;

    // for (int i = N-1; i >= 0; i--) {
    //     for (int j = 0; j < N+1; j++) {
    //         if (j > i)
    //             cout << A[i][j] << "/" << A_with_cheat[i][j];

    //         cout << "\t\t";
    //     }
    //     cout << "\t\t" << X[i] << endl;
    // }

    /** RESULT **/
    long long m = A_with_cheat[0][1];
    for (int i = 1; i < N; i++) {
        m = max(m, A_with_cheat[i][i+1]);
    }
    cout << m << endl;

    return 0;
}