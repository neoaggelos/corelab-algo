#include <iostream>
#include <vector>

using namespace std;

int N, a, b, c;
vector <long long> running_sum_X, A;

/* cost function */
inline long long costf(int from, int to) {
    long long v = (from == 0) ? (running_sum_X[to]) : (running_sum_X[to] - running_sum_X[from-1]);
    long long res = a* v *v + b*v + c;
    return res;
}

int main() {
    /* in */
    cin >> N >> a >> b >> c;

    running_sum_X.reserve(N);
    A.reserve(N+1);
    for (int i = 0; i <= N; i++) {
        A[i] = 0;
    }

    /* calculate running sum of Xs, so that we get sums fast */
    cin >> running_sum_X[0];
    for (int i = 1; i < N; i++) {
        cin >> running_sum_X[i];
        running_sum_X[i] += running_sum_X[i-1];
    }

    /* A(i) = max_k ( cost(sum_xi(i_i+k) + A(i-k) ) for k in [1, i] */
    for (int i = 1; i <= N; i++) {
        for (int k = 1; k <= i; k++) {
            long long curr = costf(N-i, N-i+k-1) + A[i - k];
            if (k == 1 || A[i] < curr)
                A[i] = curr;
        }
    }

    cout << A[N] << endl;
    return 0;
}