/*
 * Copyright ©2022 Phuoc Huynh & Isabella Bunnell.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */
#include <assert.h>
#include <iostream>
#include "Vector.h"

using vector333::Vector;
using std::cout;
using std::endl;
int main() {
    // tests default constructor
    // expected output: 
    // (0, 0, 0)
    Vector zero_vector;
    cout << zero_vector << endl;

    // checks three arg constructor
    Vector vectorA(1.0, 2.0, 3.0);
    Vector vectorB(5.0, 6.0, 7.0);
    cout << vectorA << endl;
    cout << vectorB << endl;
    // expected output:
    // (1, 2, 3)
    // (5, 6, 7)

    // checks addition overload
    Vector vectorC = vectorA + vectorB;
    cout << vectorC << endl;
    // expected output:
    // (6, 8, 10)

    // checks subtraction overload
    Vector vectorD = vectorC - vectorA;
    cout << vectorD << endl;
    // expected output:
    // (5, 6, 7)

    // checks vector multiplication overload
    float answerE = vectorD * vectorA;
    cout << answerE << endl;
    // expected output:
    // 38

    // checks scalar multiplication overload
    Vector answerF = 4 * vectorB;
    cout << answerF << endl;
    // expected output:
    // (20, 24, 28)

    // checks addition assignment overload
    vectorC += vectorD;
    cout << vectorC << endl;
    // expected output:
    // (11, 14, 17)

    // checks subtraction assignment overload
    vectorB -= vectorA;
    cout << vectorB << endl;
    // expected output:
    // (4, 4, 4)
}
