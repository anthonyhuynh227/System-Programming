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
#include "Vector.h"

// default constructor
Vector::Vector() {
    x_ = 0.0;
    y_ = 0.0;
    z_ = 0.0;
}

// defining a constructor with three arguments
Vector::Vector(const float x, const float y, const float z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

// copy constructor
Vector::Vector(const Vector &copyme) {
    x_ = copyme.x_;
    y_ = copyme.y_;
    z_ = copyme.z_;
}

// destructor
Vector::~Vector() {}

// Asignment operator
Vector &Vector::operator=(const Vector &rhs) {
    if (this != &rhs) {
        x_ = rhs.x_;
        y_ = rhs.y_;
        z_ = rhs.z_;
    }
    return *this;
}

// return a new vector that is the component-wise sum of 2 vectors
Vector &Vector::operator+=(const Vector &a) {
    if (this != &a) {
        x_ += a.x_;
        y_ += a.y_;
        z_ += a.z_;
    }
    return *this;
}

// return a new vector that is the component-wise subtraction of 2 vectors
Vector &Vector::operator-=(const Vector &b) {
    if (this != &b) {
        x_ -= b.x_;
        y_ -= b.y_;
        z_ -= b.z_;
    }
    return *this;
}

// produce the inner product of two vectors.
float Vector::vector_product(const Vector &c) const {
    return x_ * c.x_ + y_ * c.y_ + z_ * c.z_;
}
