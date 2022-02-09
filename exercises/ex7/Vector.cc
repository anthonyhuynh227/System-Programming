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
#include <iostream>

namespace vector333 {
// default constructor
Vector::Vector() {
    xyz_ = new float[3];
    xyz_[0] = 0.0;
    xyz_[1] = 0.0;
    xyz_[2] = 0.0;
}

// defining a constructor with three arguments
Vector::Vector(const float x, const float y, const float z) {
    xyz_ = new float[3];
    xyz_[0] = x;
    xyz_[1] = y;
    xyz_[2] = z;
}

// copy constructor
Vector::Vector(const Vector &copyme) {
    xyz_ = copyme.xyz_;
}

// destructor
Vector::~Vector() {
    delete[] xyz_;
}

// Asignment operator
Vector &Vector::operator=(const Vector &rhs) {
    if (this != &rhs) {
        xyz_ = rhs.xyz_;
    }
    return *this;
}

// return a new vector that is the component-wise sum of 2 vectors
Vector &Vector::operator+=(const Vector &a) {
    if (this != &a) {
        xyz_[0] += a.xyz_[0];
        xyz_[1] += a.xyz_[1];
        xyz_[2] += a.xyz_[2];
    }
    return *this;
}

// return a new vector that is the component-wise subtraction of 2 vectors
Vector &Vector::operator-=(const Vector &b) {
    if (this != &b) {
        xyz_[0] += b.xyz_[0];
        xyz_[1] += b.xyz_[1];
        xyz_[2] += b.xyz_[2];
    }
    return *this;
}

// produce the inner product of two vectors.
float operator*(const Vector &v1, const Vector &v2) {
    return v1.xyz_[0] * v2.xyz_[0] +
            v1.xyz_[1] * v2.xyz_[1] + v1.xyz_[2] * v2.xyz_[2];
}

Vector operator*(const float &k, const Vector &v1) {
    return Vector{k * v1.xyz_[0], k * v1.xyz_[1], k * v1.xyz_[2]};
}

Vector operator+(const Vector &v1, const Vector &v2) {
    return Vector{v1.xyz_[0] +
            v2.xyz_[0], v1.xyz_[1] + v2.xyz_[1], v1.xyz_[2] + v2.xyz_[2]};
}

Vector operator-(const Vector &v1, const Vector &v2) {
    return Vector{v1.xyz_[0] -
        v2.xyz_[0], v1.xyz_[1] - v2.xyz_[1], v1.xyz_[2] - v2.xyz_[2]};
}

std::ostream& operator<<(std::ostream& out, const Vector &v) {
    out << "(" << v.xyz_[0] << "," << v.xyz_[1] << "," << v.xyz_[2] << ")";
    return out;
}
}  // namespace vector333
