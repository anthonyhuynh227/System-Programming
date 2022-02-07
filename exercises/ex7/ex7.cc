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

// Increments the value stored in the vector by one.
void Increment(Vector x);
// verify that the address of the aliased vector has the same
// numeric value as the passed-in void*.
bool VerifyAddress(const Vector &x, void *y);

int main() {
    // checks default constructor
    Vector vectorA;
    assert(vectorA.get_x() == 0.0);
    assert(vectorA.get_y() == 0.0);
    assert(vectorA.get_z() == 0.0);
    // checks constructor with three arguments
    Vector vectorB(1.0, 2.0, 3.0);
    assert(vectorB.get_x() == 1.0);
    assert(vectorB.get_y() == 2.0);
    assert(vectorB.get_z() == 3.0);
    // checks copy constructos
    Vector vectorC(vectorB);
    assert(vectorC.get_x() == 1.0);
    assert(vectorC.get_y() == 2.0);
    assert(vectorC.get_z() == 3.0);
    // checks = asignment operator
    vectorC = vectorA;
    assert(vectorC.get_x() == 0.0);
    assert(vectorC.get_y() == 0.0);
    assert(vectorC.get_z() == 0.0);
    // checks += asignment operator
    vectorC += vectorB;
    assert(vectorC.get_x() == 1.0);
    assert(vectorC.get_y() == 2.0);
    assert(vectorC.get_z() == 3.0);
    // checks += asignment operator
    vectorC -= vectorB;
    assert(vectorC.get_x() == 0.0);
    assert(vectorC.get_y() == 0.0);
    assert(vectorC.get_z() == 0.0);
    // checks products of 2 vectors
    Vector vectorD = Vector(1.0, 1.0, 1.0);
    assert(6.0 == (vectorD.vector_product(vectorB)));

    // checks Increment
    Increment(vectorC);
    if (vectorC.get_x() == 1.0) {
        printf("Vector: pass-by-reference\n");
    } else {
        printf("Vector: pass-by-value\n");
    }

    // checks verify address
    if (VerifyAddress(vectorB, &vectorB)) {
        printf("Ref: same address\n");
    } else {
        printf("Ref: different address\n");
    }
}

void Increment(Vector x) {
    Vector a(1.0, 1.0, 1.0);
    x += a;
}

bool VerifyAddress(const Vector &x, void *y) {
    Vector *a = reinterpret_cast<Vector *>(y);
    Vector z = *a;
    return ((x.get_x() == z.get_x()) && (x.get_y()
            == z.get_y()) && (x.get_z() == z.get_z()));
}
