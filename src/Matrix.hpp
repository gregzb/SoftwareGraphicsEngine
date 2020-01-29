#pragma once

#include "Vec.hpp"
#include <vector>
#include <string>

enum class CurveType {
    Hermite,
    Bezier
};

class Matrix
{
private:
    std::vector<std::vector<double>> m;
    int rows, cols;

public:
    Matrix(int rows, int cols);
    Matrix(std::vector<std::vector<double>> m);
    std::vector<double> &operator[](int idx);
    int getRows() const;
    int getColumns() const;
    std::string toString();

    void identity();
    Matrix transpose();
    void clear();
    Matrix multiply(const Matrix &matrix);
    void addPoint(double x, double y, double z);
    void addPoint(Vec v);
    void addEdge(double x0, double y0, double z0, double x1, double y1, double z1);
    void addEdge(Vec v0, Vec v1);
    void addTriangle(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);
    void addTriangle(Vec v0, Vec v1, Vec v2);
    void addCircle(double x, double y, double z, double r, int steps);
    void addCircle(Vec v, double r, int steps);
    void addCurve(double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1, int steps, CurveType curveType);
    void addBox(double x, double y, double z, double w, double h, double d);
    void addBox(Vec v, double w, double h, double d);
    void addSphere(double x, double y, double z, double r, int thetaSteps, int phiSteps);
    void addSphere(Vec v, double r, int thetaSteps, int phiSteps);
    void addTorus(double x, double y, double z, double r1, double r2, int thetaSteps, int phiSteps);
    void addTorus(Vec v, double r1, double r2, int thetaSteps, int phiSteps);

    Vec getTriangleNormal(int idx);

    static Matrix rotX(double theta);
    static Matrix rotY(double theta);
    static Matrix rotZ(double theta);
    static Matrix translate(double x, double y, double z);
    static Matrix scale(double x, double y, double z);
};