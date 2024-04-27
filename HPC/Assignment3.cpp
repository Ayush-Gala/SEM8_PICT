#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to perform parallel reduction for minimum value
template <typename T>
T parallelMin(const vector<T> &arr)
{
    T result = arr[0];
#pragma omp parallel for reduction(min : result)
    for (int i = 0; i < arr.size(); ++i)
    {
        result = min(result, arr[i]);
    }
    return result;
}

// Function to perform parallel reduction for maximum value
template <typename T>
T parallelMax(const vector<T> &arr)
{
    T result = arr[0];
#pragma omp parallel for reduction(max : result)
    for (int i = 0; i < arr.size(); ++i)
    {
        result = max(result, arr[i]);
    }
    return result;
}

// Function to perform parallel reduction for sum
template <typename T>
T parallelSum(const vector<T> &arr)
{
    T result = arr[0];
#pragma omp parallel for reduction(+ : result)
    for (int i = 0; i < arr.size(); ++i)
    {
        result += arr[i];
    }
    return result;
}

// Function to perform parallel reduction for average
template <typename T>
double parallelAverage(const vector<T> &arr)
{
    T sum = parallelSum(arr);
    return static_cast<double>(sum) / arr.size();
}

int main()
{
    const int size = 1000;
    vector<int> arr(size);

    // Initialize the array with random values
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % 10000;
        cout << arr[i] << " ";
    }
    cout << endl;

    // Compute min, max, sum, and average using parallel reduction
    auto start = high_resolution_clock::now();
    int minVal = parallelMin(arr);
    auto stop = high_resolution_clock::now();
    auto parallel_min_duration = duration_cast<milliseconds>(stop-start);

    start = high_resolution_clock::now();
    int maxVal = parallelMax(arr);
    stop = high_resolution_clock::now();
    auto max_val_duration = duration_cast<milliseconds>(stop-start);

    start = high_resolution_clock::now();
    int sum = parallelSum(arr);
    stop = high_resolution_clock::now();
    auto average_val_duration = duration_cast<milliseconds>(stop-start);
    double average = parallelAverage(arr);

    cout << "Minimum value: " << minVal << endl;
    cout << "Maximum value: " << maxVal << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << average << endl;
    cout<<endl<<"Time taken for each execution are as follows: "<<endl<<"Min parallel: "<<parallel_min_duration.count()<<endl<<"Max parallel: "<<max_val_duration.count()<<endl<<"Average parallel: "<<average_val_duration.count()<<endl;

    return 0;
}
