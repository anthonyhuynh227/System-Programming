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
    // checks default constructor
    Vector vectorA(1.0, 2.0, 3.0);
    Vector vectorB(5.0, 6.0, 7.0);
    Vector vectorC = vectorA + vectorB;
    cout << vectorC << endl;

    Vector vectorD = vectorC - vectorA;
    cout << vectorD << endl;

    float answerE = vectorD * vectorA;
    cout << answerE << endl;

    Vector answerF = 4 * vectorB;
    cout << answerF << endl;
}
