#include<iostream>

void sortDescending(int A[], int n) {
    int imaks, temp;

    for (int i = n-1; i >= 1; --i) { // pass sebanyak n-1 kali
        imaks = 0;

        for (int j = 1; j <= i; ++j) {
            if (A[j] > A[imaks]) {
                imaks = j;
            }
        }

        // pertukaran aimaks dengan ai
        temp = A[i];
        A[i] = A[imaks];
        A[imaks] = temp;
    }
}

int main() {
    int A[] = {34, 7, 23, 32, 5, 62, 12, 9, 17, 45};
    int n = sizeof(A) / sizeof(A[0]);

    sortDescending(A, n);

    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << " ";
    }

    return 0;
}