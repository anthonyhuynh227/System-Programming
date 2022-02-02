#ifndef VECTOR_H_
#define VECTOR_H_

class Vector
{
public:
    Vector();                                            // default constructor
    Vector(const float x, const float y, const float z); // constructor with three arguments
    Vector(const Vector &copyme);                        // copy constructor
    float get_x() const { return x_; }                   // inline member function
    float get_y() const { return y_; }                   // inline member function
    float get_z() const { return z_; }                   // inline member function
    ~Vector();                                           // destructor
    Vector &operator=(const Vector &rhs);                // asginment operator
    Vector &operator+=(const Vector &a);                 // addtion of the vector
    Vector &operator-=(const Vector &b);                 // subtraction of the vector
    float vector_product(const Vector &c) const;         // produce the inner product of two vectors.
private:
    float x_; // data member
    float y_; // data member
    float z_; // data member
};            // class Vector

#endif // VECTOR_H_./ex