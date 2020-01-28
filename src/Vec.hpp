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
    Vec scale(double a);
    Vec cross(Vec other);
    Vec add(Vec other);
    Vec sub(Vec other);
};