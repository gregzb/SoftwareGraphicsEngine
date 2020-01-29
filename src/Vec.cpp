#include <iostream>
#include <cmath>
#include <limits>
#include "Vec.hpp"

Vec::Vec(int size)
{
    for (int i = 0; i < size; i++) {
        values.push_back(i);
    }
}

Vec::Vec(std::vector<double> values)
{
    this->values = values;
}

double &Vec::operator[](int idx)
{
    return values[idx];
}

int Vec::dimensions()
{
    return values.size();
}

double Vec::magnitude()
{
    double temp = 0;
    for (int i = 0; i < dimensions(); i++) {
        temp += values[i] * values[i];
    }
    return std::sqrt(temp);
}

Vec Vec::normalize()
{
    Vec copy(values);
    double mag = magnitude();
    if (std::abs(mag) < std::numeric_limits<double>::epsilon() * 10) {
        return Vec(dimensions());
    }
    for (int i = 0; i < dimensions(); i++) {
        copy[i] /= mag;
    }
    return copy;
}

Vec Vec::negate()
{
    Vec copy(values);
    for (int i = 0; i < dimensions(); i++) {
        copy[i] -= -values[i];
    }
    return copy;
}

double Vec::dot(Vec other)
{
    if (dimensions() != other.dimensions())
    {
        std::cout << "Vector size must be same for dot product." << std::endl;
        return 0;
    }

    double temp = 0;
    for (int i = 0; i < dimensions(); i++) {
        temp += values[i] * other[i];
    }
    return temp;
}

Vec Vec::cross(Vec other)
{
    if (dimensions() != 3 || other.dimensions() != 3)
    {
        std::cout << "Vector size must be 3 for cross product." << std::endl;
        return *this;
    }

    return {{values[1] * other[2] - values[2] * other[1],
             values[2] * other[0] - values[0] * other[2],
             values[0] * other[1] - values[1] * other[0]}};
}

Vec Vec::scale(Vec a) {
    if (dimensions() != a.dimensions())
    {
        std::cout << "Vector size must be same for add." << std::endl;
        return *this;
    }

    Vec temp(dimensions());
    for (int i = 0; i < dimensions(); i++) {
        temp[i] = values[i] * a[i];
    }
    return temp;
}

Vec Vec::scale(double a) {
    Vec temp(dimensions());
    for (int i = 0; i < dimensions(); i++) {
        temp[i] = values[i] * a;
    }
    return temp;
}

Vec Vec::add(Vec other)
{
    if (dimensions() != other.dimensions())
    {
        std::cout << "Vector size must be same for add." << std::endl;
        return *this;
    }

    Vec temp(dimensions());
    for (int i = 0; i < dimensions(); i++) {
        temp[i] = values[i] + other[i];
    }
    return temp;
}

Vec Vec::add(double other)
{
    Vec temp(dimensions());
    for (int i = 0; i < dimensions(); i++) {
        temp[i] = values[i] + other;
    }
    return temp;
}

Vec Vec::sub(Vec other)
{
    if (dimensions() != other.dimensions())
    {
        std::cout << "Vector size must be same for sub." << std::endl;
        return *this;
    }

    Vec temp(dimensions());
    for (int i = 0; i < dimensions(); i++) {
        temp[i] = values[i] - other[i];
    }
    return temp;
}

Vec Vec::sub(double other)
{
    Vec temp(dimensions());
    for (int i = 0; i < dimensions(); i++) {
        temp[i] = values[i] - other;
    }
    return temp;
}