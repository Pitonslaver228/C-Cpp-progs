#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateMatrix(int arr[], int size) {
    for(int i = 0; i < size; i++){

        arr[i] = rand() % 100;
    }
}

void printMatrix(int arr[], int M, int N) {
    for(int i = 0; i < M; i++) {

        for(int j = 0; j < N; j++) {
            printf("%d ", arr[i * N + j]);
        }

        printf("\n");
    }
    printf("--------------------------\n");
}

int sumOfDigits(int num) {
    int sum = 0;
    while(num >= 1) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

void copyMatrix(int arr_2[], int arr_1[], int size) {
    for(int i = 0; i < size; i++){
        arr_2[i] = arr_1[i];
    }
}

void bubbleSort(int arr[], int size, int *comparsions, int *swaps) {
    *comparsions = 0;
    *swaps = 0;

    for (int i = 0; i < size - 1; i++) {

        for (int j = 0; j < size - 1 - i; j++) {
            
            (*comparsions)++;
            if(sumOfDigits(arr[j]) < sumOfDigits(arr[j + 1])) {

                int a = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = a;
                (*swaps)++;
            }
        }
    }
}

void quickSort(int arr[], int left, int right, int *comparsions, int *swaps) {
    *comparsions = 0;
    *swaps = 0;

    if(left < right) {

        int pivot = arr[(left + right) / 2];
        int i = left;
        int j = right;

        while(i <= j) {

            while(sumOfDigits(arr[i]) > sumOfDigits(pivot)) {
                (*comparsions)++;
                i++;
            }

            while(sumOfDigits(arr[j]) < sumOfDigits(pivot)) {
                (*comparsions)++;  
                j--;
            }

            if (i <= j) {

                int change = arr[i];
                arr[i] = arr[j];
                arr[j] = change;
                (*swaps)++;
                i++;
                j--;
            }
        }

        if (left < j) {
            quickSort(arr, left, j, comparsions, swaps);
        }

        if(i < right) {
            quickSort(arr, i, right, comparsions, swaps);
        }
    }
}

void insertionSort(int arr[], int size, int *comparsions, int *swaps) {
    *comparsions = 0;
    *swaps = 0;

    for(int i = 1; i < size; i++) {

        int key = arr[i];
        int j = i - 1;

        while(sumOfDigits(arr[j]) < sumOfDigits(key) && j >= 0) {

            (*comparsions)++;
            arr[j + 1] = arr[j];
            (*swaps)++;
            j--;
        }

        arr[j + 1] = key;
    }
}

void shellSort(int arr[], int size, int *comparsions, int *swaps) {
    *comparsions = 0;
    *swaps = 0;

    for (int step = size / 2; step > 0; step /= 2) {  

        for (int i = step; i < size; i++) {

            int change = arr[i];
            int j;

            for (j = i; j >= step && sumOfDigits(arr[j - step]) <= sumOfDigits(change); j -= step) {
                (*comparsions)++;
                arr[j] = arr[j - step]; 
                (*swaps)++; 
            }

            arr[j] = change; 
        }
    }
}


void selectionSort(int arr[], int size, int *comparsions, int *swaps) {
    *comparsions = 0;
    *swaps = 0;

    for (int i = 0; i < size - 1; i++) {
        int max = i; 

        for (int j = i + 1; j < size; j++) {

            (*comparsions)++;
            if (sumOfDigits(arr[j]) > sumOfDigits(arr[max])) { 
                max = j;
            }
        }
        
        int change = arr[i];
        arr[i] = arr[max];
        arr[max] = change;
        (*swaps)++;
    }
}

int main() {
    srand(time(0));
    
    int M, N;
    int comparsions, swaps;

    printf("Enter M: ");
    scanf("%d", &M);
    printf("--------------------------\n");
    printf("Enter N: ");
    scanf("%d", &N);
    printf("--------------------------\n");

    int size = M * N;
    int arr_start[size], arr_sorting[size]; 

    printf("Start matrix:\n");
    generateMatrix(arr_start, size);  
    printMatrix(arr_start, M, N);

    printf("bubbleSort matrix:\n");
    copyMatrix(arr_sorting, arr_start, size); 
    bubbleSort(arr_sorting, size, &comparsions, &swaps);
    printf("Comparsions: %d, swaps: %d\n", comparsions, swaps);
    printMatrix(arr_sorting, M, N);

    printf("quickSort matrix:\n");
    copyMatrix(arr_sorting, arr_start, size);
    quickSort(arr_sorting, 0, size - 1, &comparsions, &swaps);
    printf("Comparsions: %d, swaps: %d\n", comparsions, swaps); 
    printMatrix(arr_sorting, M, N);

    printf("insertSort matrix:\n");
    copyMatrix(arr_sorting, arr_start, size);
    insertionSort(arr_sorting, size, &comparsions, &swaps);
    printf("Comparsions: %d, swaps: %d\n", comparsions, swaps);
    printMatrix(arr_sorting, M, N);

    printf("shellSort matrix:\n");
    copyMatrix(arr_sorting, arr_start, size);
    shellSort(arr_sorting, size, &comparsions, &swaps);
    printf("Comparsions: %d, swaps: %d\n", comparsions, swaps);
    printMatrix(arr_sorting, M, N);

    printf("selectionSort matrix:\n");
    copyMatrix(arr_sorting, arr_start, size);
    selectionSort(arr_sorting, size, &comparsions, &swaps);
    printf("Comparsions: %d, swaps: %d\n", comparsions, swaps);
    printMatrix(arr_sorting, M, N);

    return 0;
}
