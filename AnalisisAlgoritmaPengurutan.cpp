#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cstdlib>
#include <random>

using namespace std;

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

long long measureTime(void (*sortFunction)(vector<int>&), vector<int>& arr) {
    auto start = chrono::high_resolution_clock::now();
    sortFunction(arr);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> duration = end - start;
    return duration.count();
}

vector<int> generateRandomData(int n) {
    vector<int> data(n);
    generate(data.begin(), data.end(), rand);
    return data;
}

vector<int> generateReversedData(int n) {
    vector<int> data(n);
    iota(data.rbegin(), data.rend(), 1);
    return data;
}

vector<int> generateSortedData(int n) {
    vector<int> data(n);
    iota(data.begin(), data.end(), 1);
    return data;
}

void printTableHeader(const string& condition) {
    cout << setw(20) << "Algoritma" << setw(20) << "Kompleksitas Waktu"
         << setw(15) << "N=10" << setw(15) << "N=100"
         << setw(15) << "N=500" << setw(15) << "N=1000"
         << setw(15) << "N=10000" << endl;
}

void printTableRow(const string& name, const string& complexity,
                   const vector<long long>& times) {
    cout << setw(20) << name << setw(20) << complexity;
    for (auto time : times) {
        cout << setw(15) << time;
    }
    cout << endl;
}

int main() {
    vector<int> sizes = {10, 100, 500, 1000, 10000};
    vector<string> algorithms = {"Bubble Sort", "Insertion Sort", "Selection Sort", "Merge Sort", "Quick Sort"};
    vector<string> complexities = {"O(n^2)", "O(n^2)", "O(n^2)", "O(n log n)", "O(n log n)"};
    vector<void (*)(vector<int>&)> functions = {bubbleSort, insertionSort, selectionSort,
                                                [](vector<int>& arr) {mergeSort(arr, 0, arr.size() - 1);},
                                                [](vector<int>& arr) {quickSort(arr, 0, arr.size() - 1);}};

    
    vector<string> conditions = {"Random", "Reversed", "Sorted"};
    vector<vector<int>(*)(int)> dataGenerators = {generateRandomData, generateReversedData, generateSortedData};

    for (size_t condIdx = 0; condIdx < conditions.size(); ++condIdx) {
        cout << "\n" << string(80, '=') << "\n";
        cout << "Hasil Pengujian untuk Data: " << conditions[condIdx] << "\n";
        cout << string(80, '=') << "\n";
        printTableHeader(conditions[condIdx]);

        for (size_t i = 0; i < algorithms.size(); i++) {
            vector<long long> times;
            for (int size : sizes) {
                vector<int> data = dataGenerators[condIdx](size);
                times.push_back(measureTime(functions[i], data));
            }
            printTableRow(algorithms[i], complexities[i], times);
        }
    }

    return 0;
}
