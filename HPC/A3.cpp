#include <iostream>
#include <omp.h>

using namespace std;

// Function to find minimum value using Parallel Reduction
int minval(int arr[], int n) {
    int min_value = arr[0];

    #pragma omp parallel for reduction(min : min_value)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_value)
            min_value = arr[i];
    }

    return min_value;
}

// Function to find maximum value using Parallel Reduction
int maxval(int arr[], int n) {
    int max_value = arr[0];

    #pragma omp parallel for reduction(max : max_value)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_value)
            max_value = arr[i];
    }

    return max_value;
}

// Function to find sum using Parallel Reduction
int sum(int arr[], int n) {
    int total = 0;

    #pragma omp parallel for reduction(+ : total)
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }

    return total;
}

// Function to calculate average
double average(int arr[], int n) {
    return (double)sum(arr, n) / n;
}

int main() {

    int n = 5;
    int arr[] = {1, 2, 3, 4, 5};

    cout << "Array Elements: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\n\n";
    cout << "Minimum Value : " << minval(arr, n) << endl;
    cout << "Maximum Value : " << maxval(arr, n) << endl;
    cout << "Sum            : " << sum(arr, n) << endl;
    cout << "Average        : " << average(arr, n) << endl;

    return 0;
}
