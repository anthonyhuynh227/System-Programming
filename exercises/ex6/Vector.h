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

class Vector {
 public:
    Vector();               // default constructor
    // constructor with three arguments
    Vector(const float x, const float y, const float z);
    Vector(const Vector &copyme);           // copy constructor
    float get_x() const { return x_; }      // inline member function
    float get_y() const { return y_; }      // inline member function
    float get_z() const { return z_; }      // inline member function
    ~Vector();                              // destructor
    Vector &operator=(const Vector &rhs);   // asginment operator
    Vector &operator+=(const Vector &a);    // addtion of the vector
    Vector &operator-=(const Vector &b);    // subtraction of the vector
    // produce the inner product of two vectors.
    float vector_product(const Vector &c) const;
 private:
    float x_;  // data member
    float y_;  // data member
    float z_;  // data member
};             // class Vector

#endif  // VECTOR_H_./ex
