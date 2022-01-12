#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

// Copyright 2022 Bunnell, Huynh
// Isabella Bunnell - ibunnell@uw.edu
// Phuoc Huynh - phuynh08@uw.edu

void DumpBytes(void* pData, int byteLen);
int CopyAndSort(uint8_t arr1[], uint8_t arr2[], int arrSize);
int InsertionSort(uint8_t arr[], int arrSize);

int main(int argc, char* argv[]) {
  int32_t int_val = 1;
  float   float_val = 1.0f;
  uint8_t arr_unsorted[] = {3, 2, 0, 8, 17, 6, 10, 7, 8, 1, 12};
  uint8_t arr_sorted[]   = {0, 0, 0, 0,  0, 0,  0, 0, 0, 0,  0};

  DumpBytes(&int_val, sizeof(int_val));
  DumpBytes(&float_val, sizeof(float_val));
  DumpBytes(arr_unsorted, sizeof(arr_unsorted));
  CopyAndSort(arr_unsorted, arr_sorted, sizeof(arr_unsorted));
  DumpBytes(arr_sorted, sizeof(arr_unsorted));

  return EXIT_SUCCESS;
}

void DumpBytes(void* pData, int32_t byteLen) {
  if (byteLen < 0 || pData == NULL) {
       exit(-1);
    }
    // cast pData to type that takes each byte as one array entry
    uint8_t* ptr = (uint8_t*) pData;
    printf("The %d bytes starting at %p are:", byteLen, pData);

    int counter = 1;
    uint8_t byte;
    while (counter <= byteLen) {
        byte = *(ptr++);
        printf(" %02x", byte);
        counter++;
    }

    printf("\n");
}

int CopyAndSort(uint8_t arr1[], uint8_t arr2[], int arrSize) {
    if (arr1 == NULL || arr2 == NULL || arrSize < 0) {
        return EXIT_FAILURE;
    }

    DumpBytes(arr1, sizeof(uint8_t));
    InsertionSort(arr1, arrSize);
    // Copy array
    for (int i = 0; i < arrSize; i++) {
        arr2[i] = arr1[i];
    }

    return EXIT_SUCCESS;
}

int InsertionSort(uint8_t arr[], int arrSize) {
    if (arr == NULL || arrSize < 0) {
        return EXIT_FAILURE;
    }

    int i, j, compare;
    for (i = 1; i < arrSize; i++) {
        compare = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > compare) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = compare;
    }
    return EXIT_SUCCESS;
}

