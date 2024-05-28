#include <iostream>
using namespace std;

void insertionSort(int A[], int length) {
    for (int j = 1; j < length; j++) {
        int key = A[j];
        // Insert A[j] into the sorted sequence A[0..j-1].
        int i = j - 1;
        while (i >= 0 && A[i] > key) {
            A[i + 1] = A[i];
            i = i - 1;
        }
        A[i + 1] = key;
    }
}

int main() {
    int A[] = {5, 2, 4, 6, 1, 3};
    int length = sizeof(A)/sizeof(A[0]);

    insertionSort(A, length);

    // Print sorted array
    for(int i = 0; i < length; i++)
        cout << A[i] << " ";

    return 0;
}
