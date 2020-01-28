#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <limits>

#include "Utils.hpp"
#include "Matrix.hpp"
#include "Vec.hpp"
#include "PixelGrid.hpp"
#include "Screen.hpp"

// int lerp(double a, double b, double t)
// {
//     return (1 - t) * a + t * b;
// }

void parse(std::string fileName, Screen &screen, Matrix &edges, Matrix &triangles, std::vector<Matrix> &coordSystems)
{
    std::ifstream infile(fileName);

    std::cout << infile.is_open() << std::endl;

    std::string line;

    Vec offset;

    while (std::getline(infile, line))
    {
        std::cout << "command: " << line << std::endl;
        if (line == "line")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x0, y0, z0, x1, y1, z1;
            iss >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
            edges.addEdge(x0, y0, z0, x1, y1, z1);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(edges);
            edges = topCopy;
            screen.graphics.drawEdges(edges, {255, 255, 255, 255});
            edges.clear();
        }
        else if (line == "triangle")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x0, y0, z0, x1, y1, z1, x2, y2, z2;
            iss >> x0 >> y0 >> z0 >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            triangles.addTriangle(x0, y0, z0, x1, y1, z1, x2, y2, z2);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(triangles);
            triangles = topCopy;
            screen.graphics.drawTriangles(triangles, {255, 255, 255, 255});
            triangles.clear();
        }
        else if (line == "movescreen")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x, y, z;
            iss >> x >> y >> z;
            offset = {{x, y, z}};
        }
        else if (line == "scale")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x, y, z;
            iss >> x >> y >> z;
            Matrix temp = Matrix::scale(x, y, z);
            // temp.multiply(coordSystems.back());
            // coordSystems.back() = temp;
            coordSystems.back().multiply(temp);
        }
        else if (line == "move" || line == "translate")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x, y, z;
            iss >> x >> y >> z;
            Matrix temp = Matrix::translate(x, y, z);
            // temp.multiply(coordSystems.back());
            // coordSystems.back() = temp;
            coordSystems.back().multiply(temp);
        }
        else if (line == "rotate")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            char axis;
            double angle;
            iss >> axis >> angle;

            double PI = std::atan(1) * 4;

            angle *= PI / 180;
            Matrix temp(4, 4);
            temp.identity();
            if (axis == 'x')
            {
                temp = Matrix::rotX(angle);
            }
            else if (axis == 'y')
            {
                temp = Matrix::rotY(angle);
            }
            else if (axis == 'z')
            {
                temp = Matrix::rotZ(angle);
            }
            //temp.multiply(coordSystems.back());
            //coordSystems.back() = temp;
            coordSystems.back().multiply(temp);
        }
        else if (line == "display")
        {
            // screen.graphics.clear({0, 0, 0, 255});
            // screen.graphics.drawEdges(edges, {255, 255, 255, 255});
            // Matrix copy = triangles;
            // for (int row = 0; row < 3; row++) {
            //     for (int thing = 0; thing < copy[row].size(); thing++) {
            //         copy[row][thing] += offset[row];
            //     }
            // }
            // screen.graphics.drawTriangles(copy, {255, 0, 255, 255});
            screen.display();
            std::cout << std::endl;
        }
        else if (line == "save")
        {
            std::getline(infile, line);
            // screen.graphics.clear({0, 0, 0, 255});
            // screen.graphics.drawEdges(edges, {255, 255, 255, 255});
            // Matrix copy = triangles;
            // for (int row = 0; row < 3; row++) {
            //     for (int thing = 0; thing < copy[row].size(); thing++) {
            //         copy[row][thing] += offset[row];
            //     }
            // }
            // screen.graphics.drawTriangles(copy, {255, 0, 255, 255});
            screen.toFileExtension(line);
        }
        else if (line == "circle")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x, y, z, r;
            iss >> x >> y >> z >> r;
            edges.addCircle(x, y, z, r, 100);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(edges);
            edges = topCopy;
            screen.graphics.drawEdges(edges, {255, 255, 255, 255});
            edges.clear();
        }
        else if (line == "hermite")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x0, y0, x1, y1, rx0, ry0, rx1, ry1;
            iss >> x0 >> y0 >> x1 >> y1 >> rx0 >> ry0 >> rx1 >> ry1;
            edges.addCurve(x0, y0, x1, y1, rx0, ry0, rx1, ry1, 20, CurveType::Hermite);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(edges);
            edges = topCopy;
            screen.graphics.drawEdges(edges, {255, 255, 255, 255});
            edges.clear();
        }
        else if (line == "bezier")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x0, y0, x1, y1, x2, y2, x3, y3;
            iss >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
            edges.addCurve(x0, y0, x1, y1, x2, y2, x3, y3, 20, CurveType::Bezier);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(edges);
            edges = topCopy;
            screen.graphics.drawEdges(edges, {255, 255, 255, 255});
            edges.clear();
            //edges.addCurve(x0, y0, x3, y3, 3*(x1-x0), 3*(y1-y0), 3*(x3-x2), 3*(y3-y2), 50, CurveType::Hermite);
        }
        else if (line == "box")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x, y, z, w, h, d;
            iss >> x >> y >> z >> w >> h >> d;
            triangles.addBox(x, y, z, w, h, d);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(triangles);
            triangles = topCopy;
            screen.graphics.drawTriangles(triangles, {255, 255, 255, 255});
            triangles.clear();
        }
        else if (line == "sphere")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x, y, z, r;
            iss >> x >> y >> z >> r;

            triangles.addSphere(x, y, z, r, 30, 15);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(triangles);
            triangles = topCopy;
            Matrix copy = triangles;
            for (int row = 0; row < 3; row++)
            {
                for (int thing = 0; thing < copy[row].size(); thing++)
                {
                    copy[row][thing] += offset[row];
                }
            }
            screen.graphics.drawTriangles(copy, {255, 0, 255, 255});
            //screen.graphics.drawTriangles(zbuffer, triangles, {255, 255, 255, 255});
            triangles.clear();

            //edges.addCurve(x0, y0, x1, y1, x2, y2, x3, y3, 20, CurveType::Bezier);
            //edges.addCurve(x0, y0, x3, y3, 3*(x1-x0), 3*(y1-y0), 3*(x3-x2), 3*(y3-y2), 50, CurveType::Hermite);
        }
        else if (line == "torus")
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            double x, y, z, r1, r2;
            iss >> x >> y >> z >> r1 >> r2;

            triangles.addTorus(x, y, z, r1, r2, 30, 12);

            Matrix topCopy = coordSystems.back();
            topCopy.multiply(triangles);
            triangles = topCopy;
            Matrix copy = triangles;
            for (int row = 0; row < 3; row++)
            {
                for (int thing = 0; thing < copy[row].size(); thing++)
                {
                    copy[row][thing] += offset[row];
                }
            }
            screen.graphics.drawTriangles(copy, {255, 0, 255, 255});
            //screen.graphics.drawTriangles(zbuffer, triangles, {255, 255, 255, 255});
            triangles.clear();

            //edges.addCurve(x0, y0, x1, y1, x2, y2, x3, y3, 20, CurveType::Bezier);
            //edges.addCurve(x0, y0, x3, y3, 3*(x1-x0), 3*(y1-y0), 3*(x3-x2), 3*(y3-y2), 50, CurveType::Hermite);
        }
        else if (line == "push")
        {
            //std::cout << "top changed!" << std::endl;
            coordSystems.push_back(coordSystems.back());
        }
        else if (line == "pop")
        {
            coordSystems.pop_back();
        }
        else if (line == "clear")
        {
            screen.graphics.clear({0, 0, 0, 255});
            for (int row = 0; row < screen.zbuf().getHeight(); row++)
            {
                for (int col = 0; col < screen.zbuf().getWidth(); col++)
                {
                    screen.zbuf(row, col) = -std::numeric_limits<double>::infinity();
                }
            }
        }
        else
        {
            std::cout << "Unrecognized command!" << std::endl;
        }
        //std::cout << "top: \n" << coordSystems.back().toString() << std::endl;
    }
}

int main()
{
    Matrix edges(4, 0);
    Matrix triangles(4, 0);
    std::vector<Matrix> coordSystems;
    coordSystems.push_back(Matrix(4, 4));
    coordSystems[0].identity();

    Screen screen(500, 500);

    for (int row = 0; row < screen.zbuf().getHeight(); row++)
    {
        for (int col = 0; col < screen.zbuf().getWidth(); col++)
        {
            screen.zbuf(row, col) = -std::numeric_limits<double>::infinity();
        }
    }

    parse("script", screen, edges, triangles, coordSystems);

    return 0;
}