#pragma once

#include <vector>
#include <string>

#include "Vec3.hpp"
#include "Utils.hpp"

class Mat4
{
private:
    std::vector<std::vector<double>> m;
    int cols;

public:
    Mat4(int cols);
    Mat4(std::vector<std::vector<double>> m);
    std::vector<double> &operator[](int idx);
    int getRows() const;
    int getCols() const;
    std::string toString() const;

    Vec3 getPoint(int col);

    void clear();
    Mat4 multiply(const Mat4 &matrix);
    void multiplyMutate(const Mat4 &matrix);
    void addPoint(Vec3 v);
    void addEdge(Vec3 v0, Vec3 v1);
    void addTriangle(Vec3 v0, Vec3 v1, Vec3 v2);
    void addCircle(Vec3 v, double r, int steps);
    void addCurve(double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1, int steps, CurveType curveType);
    void addBox(Vec3 v, Vec3 dims);
    void addSphere(Vec3 v, double r, int thetaSteps, int phiSteps);
    void addTorus(Vec3 v, double r1, double r2, int thetaSteps, int phiSteps);

    Vec3 getTriangleNormal(int idx);

    static Mat4 identity();

    static Mat4 rotX(double theta);
    static Mat4 rotY(double theta);
    static Mat4 rotZ(double theta);
    static Mat4 translate(Vec3 v);
    static Mat4 scale(Vec3 v);
};