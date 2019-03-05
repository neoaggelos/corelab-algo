#include <iostream>
#include <vector>

using namespace std;

const double err = .0001;
long long N, L, k;
vector<long> t_a, v_a, t_b, v_b;
long tmax = 0;

/* returns a time after the first collision, but before any other collision has occured */
double
get_first_collision(double low_time, double high_time, long &collA, long &collB)
{
    double mid_time = (low_time + high_time) / 2;
    double maxposA = -1, maxposB = L + 1;

    for (long i = 0; i < N; i++)
    {
        double posA = v_a[i] * max(mid_time - t_a[i], 0.0);
        double posB = L - v_b[i] * max(mid_time - t_b[i], 0.0);

        /* someone from A or B passed out of bounds, search before */
        if (posA > L || posB < 0) {
            return get_first_collision(low_time, mid_time, collA, collB);
        }

        /* new A max */
        if (posA > maxposA) {
            maxposA = posA;
            collA = i;
        }

        /* new B max */
        if (posB < maxposB) {
            maxposB = posB;
            collB = i;
        }
    }

    // cout << low_time << "-" << mid_time << "-" << high_time << ": " << maxposA << " " << maxposB << endl;

    if (maxposA < maxposB)
        return get_first_collision(mid_time, high_time, collA, collB);

    bool A_after_B = false, B_after_A = false;
    for (long i = 0; i < N; i++)
    {
        double posA = v_a[i] * (mid_time - t_a[i]);
        double posB = L - v_b[i] * (mid_time - t_b[i]);

        /* we want exactly 1 from A to be after maxB */
        if (posA > maxposB) {
            if (A_after_B) return get_first_collision(low_time, mid_time, collA, collB);
            else A_after_B = true;
        }

        /* we want exactly 1 from B to be after maxA */
        if (posB < maxposA) {
            if (B_after_A) return get_first_collision(low_time, mid_time, collA, collB);
            else B_after_A = true;
        }
    }

    v_a[collA] = 0;
    v_b[collB] = 0;
    return mid_time;
}

int main()
{
    cin >> N >> L >> k;

    t_a.reserve(N);
    t_b.reserve(N);
    v_a.reserve(N);
    v_b.reserve(N);

    for (long i = 0; i < N; i++)
    {
        cin >> t_a[i] >> v_a[i];
        tmax = max(tmax, t_a[i]);
    }
    for (long i = 0; i < N; i++)
    {
        cin >> t_b[i] >> v_b[i];
        tmax = max(tmax, t_b[i]);
    }

    tmax += L / 2;

    double last_time = 0;
    long a, b;
    for (long i = 0; i < k; i++)
    {
        last_time = get_first_collision(last_time, tmax, a, b);

        cout << a+1 << " " << b+1 << endl;
    }
}
