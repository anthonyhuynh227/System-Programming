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

#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

namespace vector333 {
class Vector {
 public:
    Vector();  // default constructor
    // constructor with three arguments
    Vector(const float x, const float y, const float z);
    Vector(const Vector &copyme);           // copy constructor
    ~Vector();                              // destructor
    Vector &operator=(const Vector &rhs);   // asginment operator
    Vector &operator+=(const Vector &a);    // addtion of the vector
    Vector &operator-=(const Vector &b);    // subtraction of the vector
    // udating assignments on vector.
    friend Vector operator+(const Vector &v1, const Vector &v2);
    friend Vector operator-(const Vector &v1, const Vector &v2);
    // compute the inner product of two vectors.
    friend float operator*(const Vector &v1, const Vector &v2);
    // Vector multiplication.
    friend Vector operator*(const float &k, const Vector &v1);
    // stream output vector.
    friend std::ostream& operator<<(std::ostream& out, const Vector &v);
 private:
    float* xyz_;  // data member
};  // class Vector

}  // namespace vector333

#endif  // VECTOR_H_./ex
