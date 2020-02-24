#include "Mat4.hpp"
#include "Utils.hpp"
#include <string>
#include <iostream>
#include <cmath>
#include <tuple>

#include "assert.h"

std::vector<double> &Mat4::operator[](int idx)
{
    return m[idx];
}

Mat4::Mat4(int cols) : cols(cols)
{
    for (int r = 0; r < 4; r++)
    {
        m.push_back(std::vector<double>(cols));
    }
}

Mat4::Mat4(std::vector<std::vector<double>> m)
{
    assert(m.size() == 4);
    this->m = m;
    cols = m[0].size();
}

int Mat4::getRows() const
{
    return 4;
}

int Mat4::getCols() const
{
    return cols;
}

std::string Mat4::toString() const
{
    std::string temp;
    for (int row = 0; row < getRows(); row++)
    {
        for (int col = 0; col < getCols(); col++)
        {
            temp += std::to_string(m[row][col]).substr(0, 6) + " ";
        }
        if (row != getRows() - 1)
        {
            temp += "\n";
        }
    }
    return temp;
}

Vec4 Mat4::getPoint(int col) {
    return {m[0][col], m[1][col], m[2][col], m[3][col]};
}

Mat4 Mat4::identity()
{
    Mat4 temp(4);
    for (int row = 0; row < temp.getRows(); row++)
    {
        for (int col = 0; col < temp.getCols(); col++)
        {
            temp[row][col] = row == col;
        }
    }
    return temp;
}

void Mat4::clear()
{
    for (int row = 0; row < getRows(); row++)
    {
        m[row].clear();
    }
    cols = 0;
}

Mat4 Mat4::multiply(const Mat4 &matrix)
{
    assert(getCols() == matrix.getRows());

    Mat4 result(matrix.getCols());

    for (int row = 0; row < getRows(); row++)
    {
        for (int col = 0; col < matrix.getCols(); col++)
        {
            for (int i = 0; i < getCols(); i++)
            {
                result.m[row][col] += m[row][i] * matrix.m[i][col];
            }
        }
    }

    return result;
}

void Mat4::multiplyMutate(const Mat4 &matrix)
{
    assert(matrix.getCols() == getRows());

    std::vector<double> temp(4);

    for (int col = 0; col < getCols(); col++)
    {
        for (int i = 0; i < getRows(); i++)
        {
            temp[i] = m[i][col];
        }
        for (int row = 0; row < matrix.getRows(); row++)
        {
            m[row][col] = 0;
            for (int i = 0; i < getRows(); i++)
            {
                m[row][col] += matrix.m[row][i] * temp[i];
            }
            //std::cout << row << ", " << col << ": " << m[row][col] << std::endl;
        }
    }
}

void Mat4::addPoint(Vec4 v)
{
    cols += 1;
    for (int row = 0; row < getRows(); row++)
    {
        m[row].push_back(1); //adds 1, including to w
    }
    m[0][getCols() - 1] = v.x;
    m[1][getCols() - 1] = v.y;
    m[2][getCols() - 1] = v.z;
    m[2][getCols() - 1] = v.w;
}

void Mat4::addEdge(Vec4 v0, Vec4 v1)
{
    addPoint(v0);
    addPoint(v1);
}

void Mat4::addCircle(Vec4 v, double r, int steps)
{
    double step_size = 2 * M_PI / steps;

    std::vector<Vec4> vertices;

    for (int step = 0; step <= steps; step++)
    {
        vertices.push_back({r * std::cos(step_size * step) + v.x, r * std::sin(step_size * step) + v.y, v.z});
    }

    for (uint i = 0; i < vertices.size() - 1; i++)
    {
        addEdge(vertices[i], vertices[i+1]);
    }
}

void Mat4::addCurve(double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1, int steps, CurveType curveType)
{
    Mat4 points({{x0, y0},
                   {x1, y1},
                   {rx0, ry0},
                   {rx1, ry1}});

    Mat4 basisMatrix(0);

    if (curveType == CurveType::Hermite)
    {
        basisMatrix = Mat4({{2, -2, 1, 1},
                              {-3, 3, -2, -1},
                              {0, 0, 1, 0},
                              {1, 0, 0, 0}});
    }
    else
    {
        basisMatrix = Mat4({{-1, 3, -3, 1},
                              {3, -6, 3, 0},
                              {-3, 3, 0, 0},
                              {1, 0, 0, 0}});
    }

    double step_size = 1.0 / steps;

    std::vector<Mat4> vertices;

    for (int step = 0; step <= steps; step++)
    {
        double t = step_size * step;
        Mat4 T({{t * t * t, t * t, t, 1}});

        basisMatrix.multiplyMutate(T);
        points.multiplyMutate(basisMatrix);
        vertices.push_back(points); //havent tested if this works

        // T = T.multiply(basisMatrix);
        // T = T.multiply(points);
        // vertices.push_back(T);
    }

    for (uint i = 0; i < vertices.size() - 1; i++)
    {
        addEdge({vertices[i][0][0], vertices[i][0][1], 0}, {vertices[i + 1][0][0], vertices[i + 1][0][1], 0});
    }
}

Mat4 Mat4::rotX(double theta)
{
    Mat4 temp = Mat4::identity();
    temp[1][1] = std::cos(theta);
    temp[1][2] = -std::sin(theta);
    temp[2][1] = std::sin(theta);
    temp[2][2] = std::cos(theta);
    return temp;
}

Mat4 Mat4::rotY(double theta)
{
    Mat4 temp = Mat4::identity();
    temp[0][0] = std::cos(theta);
    temp[0][2] = std::sin(theta);
    temp[2][0] = -std::sin(theta);
    temp[2][2] = std::cos(theta);
    return temp;
}

Mat4 Mat4::rotZ(double theta)
{
    Mat4 temp = Mat4::identity();
    temp[0][0] = std::cos(theta);
    temp[0][1] = -std::sin(theta);
    temp[1][0] = std::sin(theta);
    temp[1][1] = std::cos(theta);
    return temp;
}

Mat4 Mat4::translate(Vec4 v)
{
    Mat4 temp = Mat4::identity();
    temp[0][3] = v.x;
    temp[1][3] = v.y;
    temp[2][3] = v.z;
    return temp;
}

Mat4 Mat4::scale(Vec4 v)
{
    Mat4 temp = Mat4::identity();
    temp[0][0] = v.x;
    temp[1][1] = v.y;
    temp[2][2] = v.z;
    return temp;
}