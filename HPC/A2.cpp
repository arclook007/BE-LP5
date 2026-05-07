#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

#define N 20

// ---------- Print Array ----------
void printArray(vector<int>& a) {
    for (int x : a)
        cout << x << " ";
    cout << endl;
}

// ---------- Sequential Bubble Sort ----------
void bubbleSeq(vector<int>& a) {
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < a.size() - i - 1; j++)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}

// ---------- Parallel Bubble Sort ----------
void bubblePar(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n; i++) {

        #pragma omp parallel for
        for (int j = i % 2; j < n - 1; j += 2) {

            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
        }
    }
}

// ---------- Merge Function ----------
void merge(vector<int>& a, int l, int m, int r) {

    vector<int> temp(r - l + 1);

    int i = l;
    int j = m + 1;
    int k = 0;

    while (i <= m && j <= r) {

        if (a[i] < a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= m)
        temp[k++] = a[i++];

    while (j <= r)
        temp[k++] = a[j++];

    for (i = 0; i < k; i++)
        a[l + i] = temp[i];
}

// ---------- Sequential Merge Sort ----------
void mergeSeq(vector<int>& a, int l, int r) {

    if (l < r) {

        int m = (l + r) / 2;

        mergeSeq(a, l, m);
        mergeSeq(a, m + 1, r);

        merge(a, l, m, r);
    }
}

// ---------- Parallel Merge Sort ----------
void mergePar(vector<int>& a, int l, int r, int depth) {

    if (l < r) {

        int m = (l + r) / 2;

        if (depth <= 0) {

            mergeSeq(a, l, m);
            mergeSeq(a, m + 1, r);

        } else {

            #pragma omp parallel sections
            {

                #pragma omp section
                mergePar(a, l, m, depth - 1);

                #pragma omp section
                mergePar(a, m + 1, r, depth - 1);
            }
        }

        merge(a, l, m, r);
    }
}

// ---------- Main ----------
int main() {

    vector<int> a(N), b;

// Fill array with random numbers
    for (int i = 0; i < N; i++) {
        a[i] = rand() % 100;
    }

    cout << "Original Array:\n";
    printArray(a);

    double start, end;
    // ---------- Sequential Bubble ----------
    
    b = a;
    start = omp_get_wtime();
    bubbleSeq(b);
    end = omp_get_wtime();
    cout << "\nSequential Bubble Sort:\n";
    printArray(b);
    cout << "Time: " << end - start << " sec\n";

    // ---------- Parallel Bubble ----------
    b = a;

    start = omp_get_wtime();
    bubblePar(b);
    end = omp_get_wtime();

    cout << "\nParallel Bubble Sort:\n";
    printArray(b);

    cout << "Time: " << end - start << " sec\n";

    // ---------- Sequential Merge ----------
    b = a;

    start = omp_get_wtime();
    mergeSeq(b, 0, N - 1);
    end = omp_get_wtime();

    cout << "\nSequential Merge Sort:\n";
    printArray(b);

    cout << "Time: " << end - start << " sec\n";

    // ---------- Parallel Merge ----------
    b = a;

    start = omp_get_wtime();
    mergePar(b, 0, N - 1, 4);
    end = omp_get_wtime();

    cout << "\nParallel Merge Sort:\n";
    printArray(b);

    cout << "Time: " << end - start << " sec\n";

    return 0;
}