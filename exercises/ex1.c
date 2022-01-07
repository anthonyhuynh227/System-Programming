#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Copyright 2022 Isabella Bunnell<ibunnell@uw.edu>
// Phuoc Huynh<phuynh08@uw.edu>

// EXCEPTION HANDLING:
// argument n not provided, or too many arguments provided.
//  n is not an integer
//  n > 350 (as 350+1 = 351, and 2^351 is too big to fit into 32 bits)
// PROCESS:
//  get n, check that n is valid
//  initilize final result as a double, starting at 0
//  initilize intermediate result as a double starting at 0
//  for i = 0 to i = n:
//      intermediate result = n'th term in series
//      add on intermediate result into final result
// Print out estimate of pi

extern int errno;

int main(int argc, char* argv[]) {
    // checks whether format of command is correct
    if (argc < 2) {
        printf("You must provide a value for n as an argument!\n");
        exit(-1);
    } else if (argc > 2) {
        printf("Too many arguments were passed in!\n");
        exit(-1);
    }
    // checks for whther the arguments passed in are legitimate
    // to the program
    char *ptr;
    errno = 0;
    int n = strtol(argv[1], &ptr, 10);
    if (errno != 0 || *ptr != '\0') {
        printf("n does not represent an integer.\n");
        exit(-1);
    } else if ((n+1) > __INT_MAX__) {
        // since n+1 is the largest value to be used in the program
        // this is the value we need to check goes over the max int
        printf("n is too large! Please choose a smaller number for n\n");
        exit(-1);
    } else if (n < 0) {
        printf("n must be greater than or equal to 0!\n");
        exit(-1);
    }
    // program execution
    double final = 3.0;
    double intermediate = 0.0;
    double twoi, sign;
    for (int i = 1; i <= n; i++) {
        twoi = 2 * i;
        if ((i+1) % 2 == 0) {
            sign = 1.0;
        } else {
            sign = -1.0;
        }
        intermediate = sign * (4 / (twoi * (twoi+1) * (twoi+2)));
        final = final + intermediate;
    }

    printf("Our estimate of Pi is %0.20f\n", final);
}

