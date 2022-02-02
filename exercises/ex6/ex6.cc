#include "Vector.h"
#include <iostream>
#include <assert.h>

using namespace std;

// Increments the value stored in the vector by one.
void Increment(Vector x);
// verify that the address of the aliased vector has the same
// numeric value as the passed-in void*.
bool VerifyAddress(Vector &x, void *y);

int main()
{
    // checks default constructor
    Vector vectorA;
    assert(vectorA.get_x() == 0.0);
    assert(vectorA.get_y() == 0.0);
    assert(vectorA.get_z() == 0.0);
    // checks constructor with three arguments
    Vector vectorB(1.1, 2.2, 3.3);
    printf("%6.4lf", vectorB.get_x());
    assert(vectorB.get_x() == 1.1);
    assert(vectorB.get_y() == 2.2);
    assert(vectorB.get_z() == 3.3);
    // checks copy constructos
    Vector vectorC(vectorB);
    assert(vectorC.get_x() == 1.1);
    assert(vectorC.get_y() == 2.2);
    assert(vectorC.get_z() == 3.3);
    // checks = asignment operator
    vectorC = vectorA;
    assert(vectorC.get_x() == 0.0);
    assert(vectorC.get_y() == 0.0);
    assert(vectorC.get_z() == 0.0);
    // checks += asignment operator
    vectorC += vectorB;
    assert(vectorC.get_x() == 1.1);
    assert(vectorC.get_y() == 2.2);
    assert(vectorC.get_z() == 3.3);
    // checks += asignment operator
    vectorC -= vectorB;
    assert(vectorC.get_x() == 0.0);
    assert(vectorC.get_y() == 0.0);
    assert(vectorC.get_z() == 0.0);
    // checks products of 2 vectors
    Vector vectorD = Vector(1.0, 1.0, 1.0);
    assert(6.6 == (vectorD.vector_product(vectorB)));

    // checks Increment
    Increment(vectorC);
    if (vectorC.get_x() == 1.0)
    {
        printf("Vector: pass-by-reference");
    }
    else
    {
        printf("Vector: pass-by-value");
    }

    // checks verify address
    if (VerifyAddress(vectorB, &vectorB))
    {
        printf("Ref: same address");
    }
    else
    {
        printf("Ref: different address");
    }
}

void Increment(Vector x)
{
    Vector a(1.0, 1.0, 1.0);
    x += a;
}

bool VerifyAddress(Vector &x, void *y)
{
    Vector *a = (Vector *)y;
    Vector z = *a;
    return ((x.get_x() == z.get_x()) && (x.get_y() == z.get_y()) && (x.get_z() == z.get_z()));
}