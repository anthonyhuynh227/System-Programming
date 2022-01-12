#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

void DumpBytes(void *pData, uint32_t byteLen) {
  uint8_t* ptr = (unsigned char*)pData;

  if ( ptr == NULL ) printf("%s",ptr);

  printf("The %u bytes starting at 0x%"PRIxPTR " are:",
              byteLen, (uintptr_t)ptr);
  while (byteLen-- > 0) {
    printf(" %02x", *ptr++);
  }
  printf("\n");
}

int CopyAndSort(uint8_t *source, uint8_t *dest, uint32_t len) {
  DumpBytes(source, sizeof(source));
  int i, j;
  if ( source == NULL || dest == NULL ) return 1;

  // Insertion sort
  for (i = 0; i < len; i++) {
      dest[i] = source[i];
      j = i -1;
      while ( j >= 0 && dest[j] > source[i]) {
           dest[j+1] = dest[j];
	   dest[j] = source[i];
	   j = j-1;
      }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  int32_t int_val = 1;
  float   float_val = 1.0f;
  uint8_t arr_unsorted[] = {3, 2, 0, 8, 17, 6, 10, 7, 8, 1, 12};
  uint8_t arr_sorted[]   = {0, 0, 0, 0,  0, 0,  0, 0, 0, 0,  0};

  DumpBytes(&int_val, sizeof(int_val));
  DumpBytes(&float_val, sizeof(float_val));
  DumpBytes(arr_unsorted, sizeof(arr_unsorted));
  CopyAndSort(arr_unsorted, arr_sorted, sizeof(arr_unsorted));
  DumpBytes(arr_sorted, sizeof(arr_sorted));
  
  return EXIT_SUCCESS;
}
