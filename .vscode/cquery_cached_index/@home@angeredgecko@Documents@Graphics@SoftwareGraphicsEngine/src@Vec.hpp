#pragma once

#include <vector>

class Vec
{
    private:
    std::vector<double> values;

    public:
    Vec(int size = 3);
    Vec(std::vector<double> values);
    double &operator[](int idx);
    int dimensions();
    double magnitude();
    double dot(Vec other);
    Vec normalize();
    Vec negate();
    Vec cross(Vec other);
    Vec scale(Vec other);
    Vec scale(double other);
    Vec add(Vec other);
    Vec add (double other);
    Vec sub(Vec other);
    Vec sub(double other);
};