#include "Mat4.hpp"
#include "Utils.hpp"
#include <string>
#include <iostream>
#include <cmath>
#include <tuple>
#include <limits>

#include "assert.h"

std::vector<double> &Mat4::operator[](int idx)
{
    return m[idx];
}

double Mat4::operator()(int row, int col) const
{
    return m[row][col];
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

Vec4 Mat4::getPoint(int col) const
{
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

Mat4 Mat4::multiply(const Mat4 &matrix) const
{
    assert(getCols() == matrix.getRows());

    Mat4 result(matrix.getCols());

    for (int row = 0; row < getRows(); row++)
    {
        for (int col = 0; col < matrix.getCols(); col++)
        {
            for (int i = 0; i < getCols(); i++)
            {
                //std::cout << row << " " << col << " " << i << std::endl;
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

void Mat4::addPoint(Vec4 const &v)
{
    cols += 1;
    for (int row = 0; row < getRows(); row++)
    {
        m[row].push_back(1); //adds 1, including to w
    }
    m[0][getCols() - 1] = v.getX();
    m[1][getCols() - 1] = v.getY();
    m[2][getCols() - 1] = v.getZ();
    m[3][getCols() - 1] = v.getW();
}

void Mat4::addEdge(Vec4 const &v0, Vec4 v1)
{
    addPoint(v0);
    addPoint(v1);
}

void Mat4::addCircle(Vec4 const &v, double r, int steps)
{
    double step_size = 2 * M_PI / steps;

    std::vector<Vec4> vertices;

    for (int step = 0; step <= steps; step++)
    {
        vertices.push_back({r * std::cos(step_size * step) + v.getX(), r * std::sin(step_size * step) + v.getY(), v.getZ()});
    }

    for (uint i = 0; i < vertices.size() - 1; i++)
    {
        addEdge(vertices[i], vertices[i + 1]);
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

        T = T.multiply(basisMatrix);
        T = T.multiply(points);
        vertices.push_back(T);
    }

    for (uint i = 0; i < vertices.size() - 1; i++)
    {
        addEdge({vertices[i][0][0], vertices[i][0][1], 0}, {vertices[i + 1][0][0], vertices[i + 1][0][1], 0});
    }
}

Mat4 Mat4::transpose() const
{
    Mat4 temp(4);
    for (int col = 0; col < getCols(); col++)
    {
        for (int row = 0; row < getRows(); row++)
        {
            temp[col][row] = m[row][col];
        }
    }
    return temp;
}

void Mat4::swapRows(int r0, int r1)
{
    std::swap(m[r0], m[r1]);
}

void Mat4::multiplyRow(int r, double val)
{
    for (auto &entry : m[r])
    {
        entry *= val;
    }
}

void Mat4::multiplyAdd(int src, int target, double multiple)
{
    for (unsigned int i = 0; i < m[target].size(); i++)
    {
        m[target][i] += m[src][i] * multiple;
    }
}

Mat4 Mat4::invert() const
{
    if (getRows() != getCols())
    {
        std::cout << "Only square matrices please >:(" << std::endl;
        exit(1);
    }
    // std::cout << "a" << std::endl;
    Mat4 temp(getCols() * 2);

    for (int col = 0; col < getCols(); col++)
    {
        for (int row = 0; row < getRows(); row++)
        {
            temp[row][col] = m[row][col];
            if (row == col)
            {
                temp[row][col + getCols()] = 1;
            }
        }
    }

    int row = 0;
    for (int col = 0; col < getCols(); col++)
    {
        int newPivotRow = -1;
        double maxValInCol = 0;
        ;
        for (int r = row; r < getRows(); r++)
        {
            if (std::abs(temp[r][col]) > maxValInCol)
            {
                maxValInCol = std::abs(temp[r][col]);
                newPivotRow = r;
            }
        }

        if (newPivotRow != -1)
        {
            temp.swapRows(row, newPivotRow);
            for (int currRow = row + 1; currRow < getRows(); currRow++)
            {
                double t = temp[currRow][col] / temp[row][col];
                temp.multiplyAdd(row, currRow, -t);
                temp[currRow][col] = 0;
            }
            row++;
        }
    }

    for (int row = getRows() - 1; row >= 0; row--)
    {
        double val = temp[row][row];
        for (int r2 = row - 1; r2 >= 0; r2--)
        {
            double upperVal = temp[r2][row];
            temp.multiplyAdd(row, r2, -(upperVal / val));
        }
        temp.multiplyRow(row, 1 / val);
    }

    Mat4 final(4);

    for (int col = 0; col < getCols(); col++)
    {
        for (int row = 0; row < getRows(); row++)
        {
            final[row][col] = temp[row][col + getCols()];
            //std::cout << temp[row][col + getCols()] << std::endl;
            if (row == col)
            {
                if (std::abs(temp[row][col] - 1) > 0.000001) {
                    std::cout << "inverse broke?" << std::endl;
                    exit(1);
                }
            } else {
                if (std::abs(temp[row][col]) > 0.000001) {
                    std::cout << "inverse broke?" << std::endl;
                    exit(1);
                }
            }
        }
    }

    // std::cout << "g" << std::endl;

    return final;
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

Mat4 Mat4::translate(Vec4 const &v)
{
    Mat4 temp = Mat4::identity();
    temp[0][3] = v.getX();
    temp[1][3] = v.getY();
    temp[2][3] = v.getZ();
    return temp;
}

Mat4 Mat4::scale(Vec4 const &v)
{
    Mat4 temp = Mat4::identity();
    temp[0][0] = v.getX();
    temp[1][1] = v.getY();
    temp[2][2] = v.getZ();
    return temp;
}