#include "Matrix.hpp"
#include "Vec.hpp"
#include <string>
#include <iostream>
#include <cmath>

std::vector<double> &Matrix::operator[](int idx)
{
    return m[idx];
}

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols)
{
    for (int r = 0; r < rows; r++)
    {
        m.push_back(std::vector<double>(cols));
    }
}

Matrix::Matrix(std::vector<std::vector<double>> m)
{
    this->m = m;
    rows = m.size();
    cols = m.size() > 0 ? m[0].size() : 0;
}

int Matrix::getRows() const
{
    return rows;
}

int Matrix::getColumns() const
{
    return cols;
}

std::string Matrix::toString()
{
    std::string temp;
    for (int row = 0; row < getRows(); row++)
    {
        for (int col = 0; col < getColumns(); col++)
        {
            temp += std::to_string(m[row][col]).substr(0, 6) + " ";
        }
        temp += "\n";
    }
    return temp;
}

void Matrix::identity()
{
    for (int row = 0; row < getRows(); row++)
    {
        for (int col = 0; col < getColumns(); col++)
        {
            m[row][col] = row == col;
        }
    }
}

Matrix Matrix::transpose()
{
    Matrix temp(getColumns(), getRows());
    for (int row = 0; row < getRows(); row++)
    {
        for (int col = 0; col < getColumns(); col++)
        {
            temp[col][row] = m[row][col];
        }
    }

    return temp;
}

void Matrix::clear()
{
    for (int row = 0; row < getRows(); row++)
    {
        m[row].clear();
    }
    cols = 0;
}

Matrix Matrix::multiply(const Matrix &matrix)
{
    if (getColumns() != matrix.getRows())
    {
        std::cout << "Illegal matrix multiplication!" << std::endl;
        return *this;
    }

    Matrix result(getRows(), matrix.getColumns());

    for (int row = 0; row < getRows(); row++)
    {
        for (int col = 0; col < matrix.getColumns(); col++)
        {
            for (int i = 0; i < getColumns(); i++)
            {
                result.m[row][col] += m[row][i] * matrix.m[i][col];
            }
        }
    }

    return result;
}

void Matrix::addPoint(double x, double y, double z)
{
    if (getRows() < 3)
    {
        std::cout << "Your matrix ain't big enough." << std::endl;
        return;
    }
    cols += 1;
    for (int row = 0; row < getRows(); row++)
    {
        m[row].push_back(1);
    }
    m[0][getColumns() - 1] = x;
    m[1][getColumns() - 1] = y;
    m[2][getColumns() - 1] = z;
}

void Matrix::addPoint(Vec v)
{
    Vec temp(3);
    for (int i = 0; i < v.dimensions(); i++)
    {
        temp[i] = v[i];
    }
    addPoint(temp[0], temp[1], temp[2]);
}

void Matrix::addEdge(double x0, double y0, double z0, double x1, double y1, double z1)
{
    addPoint(x0, y0, z0);
    addPoint(x1, y1, z1);
}

void Matrix::addEdge(Vec v0, Vec v1)
{
    addPoint(v0);
    addPoint(v1);
}

void Matrix::addTriangle(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2)
{
    addPoint(x0, y0, z0);
    addPoint(x1, y1, z1);
    addPoint(x2, y2, z2);
}

void Matrix::addTriangle(Vec v0, Vec v1, Vec v2)
{
    addPoint(v0);
    addPoint(v1);
    addPoint(v2);
}

void Matrix::addCircle(double x, double y, double z, double r, int steps)
{
    double step_size = 2 * M_PI / steps;

    std::vector<Matrix> vertices;

    for (int step = 0; step <= steps; step++)
    {
        vertices.push_back(Matrix({{r * std::cos(step_size * step) + x, r * std::sin(step_size * step) + y}}));
    }

    for (uint i = 0; i < vertices.size() - 1; i++)
    {
        addEdge(vertices[i][0][0], vertices[i][0][1], z, vertices[i + 1][0][0], vertices[i + 1][0][1], z);
    }
}

void Matrix::addCircle(Vec v, double r, int steps)
{
    Vec temp(3);
    for (int i = 0; i < v.dimensions(); i++)
    {
        temp[i] = v[i];
    }
    addCircle(temp[0], temp[1], temp[2], r, steps);
}

void Matrix::addCurve(double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1, int steps, CurveType curveType)
{
    Matrix points({{x0, y0},
                   {x1, y1},
                   {rx0, ry0},
                   {rx1, ry1}});

    // Matrix hermite({
    //     {2, -2, 1, 1},
    //     {-3, 3, -2, -1},
    //     {0, 0, 1, 0},
    //     {1, 0, 0, 0}
    // });

    Matrix basisMatrix(4, 4);

    if (curveType == CurveType::Hermite)
    {
        basisMatrix = Matrix({{2, -2, 1, 1},
                              {-3, 3, -2, -1},
                              {0, 0, 1, 0},
                              {1, 0, 0, 0}});
    }
    else
    {
        basisMatrix = Matrix({{-1, 3, -3, 1},
                              {3, -6, 3, 0},
                              {-3, 3, 0, 0},
                              {1, 0, 0, 0}});
    }

    double step_size = 1.0 / steps;

    std::vector<Matrix> vertices;

    for (int step = 0; step <= steps; step++)
    {
        double t = step_size * step;
        Matrix T({{t * t * t, t * t, t, 1}});

        T.multiply(basisMatrix);
        //std::cout << T.toString();
        T.multiply(points);
        //std::cout << T.toString();
        vertices.push_back(T);
        //std::cout << points.toString() << std::endl;
    }

    for (uint i = 0; i < vertices.size() - 1; i++)
    {
        addEdge(vertices[i][0][0], vertices[i][0][1], 0, vertices[i + 1][0][0], vertices[i + 1][0][1], 0);
    }

    //Matrix T({{}});
}

void Matrix::addBox(double x, double y, double z, double w, double h, double d)
{
    double x1 = x + w;
    double y1 = y - h;
    double z1 = z - d;

    addTriangle(x, y, z, x1, y, z, x, y, z1);
    addTriangle(x1, y ,z, x1, y, z1, x, y, z1);

    addTriangle(x, y, z, x, y1, z, x1, y1, z);
    addTriangle(x, y, z, x1, y1, z, x1, y, z);

    addTriangle(x, y, z, x, y1, z1, x, y1, z);
    addTriangle(x, y, z, x, y, z1, x, y1, z1);

    addTriangle(x, y1, z, x, y1, z1, x1, y1, z);
    addTriangle(x1, y1, z, x, y1, z1, x1, y1, z1);

    addTriangle(x1, y, z, x1, y1, z, x1, y1, z1);
    addTriangle(x1, y, z, x1, y1, z1, x1, y, z1);

    addTriangle(x, y, z1, x1, y1, z1, x, y1, z1);
    addTriangle(x, y, z1, x1, y, z1, x1, y1, z1);
}

void Matrix::addBox(Vec v, double w, double h, double d)
{
    Vec temp(3);
    for (int i = 0; i < v.dimensions(); i++)
    {
        temp[i] = v[i];
    }
    addBox(temp[0], temp[1], temp[2], w, h, d);
}

// void Matrix::addSphere(double x, double y, double z, double r, int thetaSteps, int phiSteps)
// {
//     double thetaStepSize = 2 * M_PI / thetaSteps;
//     double phiStepSize = M_PI / phiSteps;

//     std::vector<Matrix> vertices;

//     for (int phiStep = 0; phiStep <= phiSteps; phiStep++)
//     {
//         double phi = phiStep * phiStepSize;
//         for (int thetaStep = 0; thetaStep <= thetaSteps; thetaStep++)
//         {
//             double theta = thetaStep * thetaStepSize;
//             Matrix point({{r * std::cos(phi) + x,
//                            r * std::sin(phi) * std::cos(theta) + y,
//                            r * std::sin(phi) * std::sin(theta) + z}});
//             vertices.push_back(point);
//         }
//     }

//     for (uint i = 0; i < vertices.size(); i++)
//     {
//         addEdge(vertices[i][0][0], vertices[i][0][1], vertices[i][0][2], vertices[i][0][0], vertices[i][0][1], vertices[i][0][2]);
//     }
// }

void Matrix::addSphere(double x, double y, double z, double r, int thetaSteps, int phiSteps)
{
    double thetaStepSize = 2 * M_PI / thetaSteps;
    double phiStepSize = M_PI / phiSteps;

    std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};

    for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    {
        for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
        {
            std::vector<Vec> points;
            for (int i = 0; i < 8; i += 2)
            {
                double phi = (phiStep + offsets[i]) * phiStepSize;
                double theta = (thetaStep + offsets[i + 1]) * thetaStepSize;
                points.push_back({{r * std::cos(phi) + x,
                                   r * std::sin(phi) * std::cos(theta) + y,
                                   r * std::sin(phi) * std::sin(theta) + z}});
            }

            addTriangle(points[0], points[1], points[2]);
            addTriangle(points[0], points[2], points[3]);
        }
    }
}

void Matrix::addSphere(Vec v, double r, int thetaSteps, int phiSteps)
{
    Vec temp(3);
    for (int i = 0; i < v.dimensions(); i++)
    {
        temp[i] = v[i];
    }
    addSphere(temp[0], temp[1], temp[2], r, thetaSteps, phiSteps);
}

// void Matrix::addTorus(double x, double y, double z, double r1, double r2, int thetaSteps, int phiSteps)
// {
//     double thetaStepSize = 2 * M_PI / thetaSteps;
//     double phiStepSize = 2 * M_PI / phiSteps;
//
//     std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};
//
//     for (int phiStep = 0; phiStep < phiSteps; phiStep++)
//     {
//         for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
//         {
//             std::vector<Vec> points;
//             for (int i = 0; i < 8; i += 2)
//             {
//                 double phi = (phiStep + offsets[i]) * phiStepSize;
//                 double theta = (thetaStep + offsets[i + 1]) * thetaStepSize;
//                 points.push_back({{(r2 + r1 * std::cos(phi)) * std::cos(theta) + x,
//                                    r1 * std::sin(phi) + y,
//                                    (r2 + r1 * std::cos(phi)) * std::sin(theta) + z}});
//             }
//
//             addTriangle(points[0], points[1], points[2]);
//             addTriangle(points[0], points[2], points[3]);
//         }
//     }
// }

void Matrix::addTorus(double x, double y, double z, double r1, double r2, int thetaSteps, int phiSteps)
{
    double thetaStepSize = 2 * M_PI / thetaSteps;
    double phiStepSize = 2 * M_PI / phiSteps;

    std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};

    for (int phiStep = 0; phiStep < 1; phiStep++)
    {
        for (int thetaStep = 5; thetaStep < 6; thetaStep++)
        {
            std::vector<Vec> points;
            for (int i = 0; i < 8; i += 2)
            {
                double phi = (phiStep + offsets[i]) * phiStepSize;
                double theta = (thetaStep + offsets[i + 1]) * thetaStepSize;
                points.push_back({{(r2 + r1 * std::cos(phi)) * std::cos(theta) + x,
                                   r1 * std::sin(phi) + y,
                                   (r2 + r1 * std::cos(phi)) * std::sin(theta) + z}});
            }

            addTriangle(points[0], points[1], points[2]);
            //addTriangle(points[0], points[2], points[3]);
        }
    }
}

void Matrix::addTorus(Vec v, double r1, double r2, int thetaSteps, int phiSteps)
{
    Vec temp(3);
    for (int i = 0; i < v.dimensions(); i++)
    {
        temp[i] = v[i];
    }
    addTorus(temp[0], temp[1], temp[2], r1, r2, thetaSteps, phiSteps);
}

Vec Matrix::getTriangleNormal(int idx)
{
    Vec temps[2];
    for (int i = 0; i < 2; i++) {
        temps[i] = {{
            m[0][i + idx + 1] - m[0][i + idx],
            m[1][i + idx + 1] - m[1][i + idx],
            m[2][i + idx + 1] - m[1][i + idx]
        }};
    }
    return temps[0].cross(temps[1]);
}

Matrix Matrix::rotX(double theta)
{
    Matrix temp(4, 4);
    temp.identity();
    temp[1][1] = std::cos(theta);
    temp[1][2] = -std::sin(theta);
    temp[2][1] = std::sin(theta);
    temp[2][2] = std::cos(theta);
    return temp;
}

Matrix Matrix::rotY(double theta)
{
    Matrix temp(4, 4);
    temp.identity();
    temp[0][0] = std::cos(theta);
    temp[0][2] = std::sin(theta);
    temp[2][0] = -std::sin(theta);
    temp[2][2] = std::cos(theta);
    return temp;
}

Matrix Matrix::rotZ(double theta)
{
    Matrix temp(4, 4);
    temp.identity();
    temp[0][0] = std::cos(theta);
    temp[0][1] = -std::sin(theta);
    temp[1][0] = std::sin(theta);
    temp[1][1] = std::cos(theta);
    return temp;
}

Matrix Matrix::translate(double x, double y, double z)
{
    Matrix temp(4, 4);
    temp.identity();
    temp[0][3] = x;
    temp[1][3] = y;
    temp[2][3] = z;
    return temp;
}

Matrix Matrix::scale(double x, double y, double z)
{
    Matrix temp(4, 4);
    temp.identity();
    temp[0][0] = x;
    temp[1][1] = y;
    temp[2][2] = z;
    return temp;
}
