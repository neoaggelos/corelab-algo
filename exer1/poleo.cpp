#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    long long N;
    cin >> N;

    vector <long long> array(N);
    for (long long i = 0; i < N; i++) {
        cin >> array[i];
    }

    /* Sums from the right */
    vector <long long> right(N), right_prevmax(N);
    right[N-1] = array[N-1];
    right_prevmax[N-1] = -1;

    long long max_so_far = right[N-1];
    for (long long i = N-2; i >= 0; i--) {
        if (array[i] >= max_so_far) {
            right[i] = array[i] * (N-i);
            max_so_far = array[i];
        }
        else if (array[i] == array[i+1]) {
            right[i] = array[i] + right[i+1];
            right_prevmax[i] = right_prevmax[i+1];
        }
        else if (array[i] < array[i+1]) {
            right[i] = array[i] + right[i+1];
            right_prevmax[i] = i+1;
        }
        else /* if (array[i] > array[i+1]) */ {
            long long j = i+1;
            while (right_prevmax[j] != -1 && array[i] > array[j])
                j = right_prevmax[j];

            right_prevmax[i] = j;
            right[i] = array[i] * (j-i) + right[j];
        }
    }

    /* Sums from the left */
    vector <long long> left(N), left_prevmax(N);
    left[0] = array[0];
    left_prevmax[0] = -1;
    max_so_far = array[0];

    long long min_so_far = __LONG_LONG_MAX__;
    for (long long i = 1; i < N; i++) {
        if (array[i] >= max_so_far) {
            left[i] = array[i] * (i+1);
            max_so_far = array[i];
        }
        else if (array[i] == array[i-1]) {
            left[i] = array[i] + left[i-1];
            left_prevmax[i] = left_prevmax[i-1];
        }
        else if (array[i] < array[i-1]) {
            left[i] = array[i] + left[i-1];
            left_prevmax[i] = i-1;
        }
        else /* if (array[i] > array[i-1]) */ {
            long long j = i-1;
            while (left_prevmax[i] != -1 && array[i] > array[j])
                j = left_prevmax[j];

            left_prevmax[i] = j;
            left[i] = array[i] * (i-j) + left[j];
        }

        min_so_far = min(min_so_far, right[i] + left[i] - array[i]);
    }

    cout << min_so_far << endl;
}