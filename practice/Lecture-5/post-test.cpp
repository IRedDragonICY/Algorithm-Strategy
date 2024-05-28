#include <bits/stdc++.h>
using namespace std;

void selectionSort(int arr[], int n) {
    int i, j, min_idx;
    for (i = 0; i < n-1; i++) {
        min_idx = i;
        for (j = i+1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        swap(arr[min_idx], arr[i]);

        cout << "Langkah ke-" << i+1 << ": ";
        for (int k = 0; k < n; k++) {
            cout << arr[k] << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;
    cout << "Masukkan jumlah data yang akan di urutkan: ";
    cin >> n;
    int arr[n];

    cout << "=====================================" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Masukkan data ke-" << i+1 << ": ";
        cin >> arr[i];
    }
    cout << "=====================================" << endl;
    selectionSort(arr, n);
    cout << "=====================================" << endl;
    cout << "Sorted array: " << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}