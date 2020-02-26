#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <limits>

#include <chrono>

#include "Utils.hpp"
#include "PixelGrid.hpp"
#include "Screen.hpp"
#include "Mat4.hpp"

#include "RenderObject.hpp"
#include "Camera.hpp"

#include "OBJLoader.hpp"

// void parse(std::string fileName, Screen &screen, Mat4 &edges, Mat4 &triangles, std::vector<Mat4> &coordSystems)
// {
//     std::ifstream infile(fileName);

//     std::string line;

//     Vec3 offset;

//     while (std::getline(infile, line))
//     {
//         std::cout << "command: " << line << std::endl;
//         if (line == "line")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v0, v1;
//             iss >> v0 >> v1;
//             edges.addEdge(v0, v1);

//             //edges = coordSystems.back().multiply(edges);
//             edges.multiplyMutate(coordSystems.back());
//             screen.graphics.drawEdges(edges, {255, 255, 255, 255});
//             edges.clear();
//         }
//         else if (line == "triangle")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v0, v1, v2;
//             iss >> v0 >> v1 >> v2;
//             triangles.addTriangle(v0, v1, v2);

//             //triangles = coordSystems.back().multiply(triangles);
//             triangles.multiplyMutate(coordSystems.back());
//             screen.graphics.drawTriangles(triangles, {255, 255, 255, 255});
//             triangles.clear();
//         }
//         else if (line == "movescreen")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             double x, y, z;
//             iss >> x >> y >> z;
//             offset = {x, y, z};
//         }
//         else if (line == "scale")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v;
//             iss >> v;
//             Mat4 temp = Mat4::scale(v);
//             // temp.multiply(coordSystems.back());
//             // coordSystems.back() = temp;
//             coordSystems.back() = coordSystems.back().multiply(temp);
//         }
//         else if (line == "move" || line == "translate")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v;
//             iss >> v;
//             Mat4 temp = Mat4::translate(v);
//             // temp.multiply(coordSystems.back());
//             // coordSystems.back() = temp;
//             coordSystems.back() = coordSystems.back().multiply(temp);
//         }
//         else if (line == "rotate")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             char axis;
//             double angle;
//             iss >> axis >> angle;

//             double PI = std::atan(1) * 4;

//             angle *= PI / 180;
//             Mat4 temp(0);
//             temp.identity();
//             if (axis == 'x')
//             {
//                 temp = Mat4::rotX(angle);
//             }
//             else if (axis == 'y')
//             {
//                 temp = Mat4::rotY(angle);
//             }
//             else if (axis == 'z')
//             {
//                 temp = Mat4::rotZ(angle);
//             }
//             //temp.multiply(coordSystems.back());
//             //coordSystems.back() = temp;
//             coordSystems.back() = coordSystems.back().multiply(temp);
//         }
//         else if (line == "display")
//         {
//             screen.display();
//             std::cout << std::endl;
//         }
//         else if (line == "save")
//         {
//             std::getline(infile, line);
//             std::cout << "file name " << line << std::endl;
//             screen.toFileExtension(line);
//         }
//         else if (line == "circle")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v;
//             double r;
//             iss >> v >> r;
//             edges.addCircle(v, r, 100);

//             //edges = coordSystems.back().multiply(edges);
//             edges.multiplyMutate(coordSystems.back());
//             screen.graphics.drawEdges(edges, {255, 255, 255, 255});
//             edges.clear();
//         }
//         else if (line == "hermite")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             double x0, y0, x1, y1, rx0, ry0, rx1, ry1;
//             iss >> x0 >> y0 >> x1 >> y1 >> rx0 >> ry0 >> rx1 >> ry1;
//             edges.addCurve(x0, y0, x1, y1, rx0, ry0, rx1, ry1, 20, CurveType::Hermite);

//             //edges = coordSystems.back().multiply(edges);
//             edges.multiplyMutate(coordSystems.back());
//             screen.graphics.drawEdges(edges, {255, 255, 255, 255});
//             edges.clear();
//         }
//         else if (line == "bezier")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             double x0, y0, x1, y1, x2, y2, x3, y3;
//             iss >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
//             edges.addCurve(x0, y0, x1, y1, x2, y2, x3, y3, 20, CurveType::Bezier);

//             //edges = coordSystems.back().multiply(edges);
//             edges.multiplyMutate(coordSystems.back());
//             screen.graphics.drawEdges(edges, {255, 255, 255, 255});
//             edges.clear();
//             //edges.addCurve(x0, y0, x3, y3, 3*(x1-x0), 3*(y1-y0), 3*(x3-x2), 3*(y3-y2), 50, CurveType::Hermite);
//         }
//         else if (line == "box")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v, dims;
//             iss >> v >> dims;
//             triangles.addBox(v, dims);

//             //triangles = coordSystems.back().multiply(triangles);
//             triangles.multiplyMutate(coordSystems.back());
//             screen.graphics.drawTriangles(triangles, {255, 255, 255, 255});
//             triangles.clear();
//         }
//         else if (line == "sphere")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v;
//             double r;
//             iss >> v >> r;

//             triangles.addSphere(v, r, 30, 15);
//             // triangles.addSphere(v, r, 3000, 1500);

//             //std::cout << triangles.getCols() << std::endl;

//             //triangles = coordSystems.back().multiply(triangles);
//             triangles.multiplyMutate(coordSystems.back());
//             screen.graphics.drawTriangles(triangles, {255, 0, 255, 255});
//             triangles.clear();
//         }
//         else if (line == "torus")
//         {
//             std::getline(infile, line);
//             std::istringstream iss(line);

//             Vec3 v;
//             double r1, r2;
//             iss >> v >> r1 >> r2;

//             triangles.addTorus(v, r1, r2, 30, 12);

//             //triangles = coordSystems.back().multiply(triangles);
//             triangles.multiplyMutate(coordSystems.back());
//             for (int i = 0; i < triangles.getCols(); i++)
//             {
//                 triangles[0][i] += offset.x;
//                 triangles[1][i] += offset.y;
//                 triangles[2][i] += offset.z;
//             }
//             screen.graphics.drawTriangles(triangles, {255, 0, 255, 255});
//             triangles.clear();
//         }
//         else if (line == "push")
//         {
//             coordSystems.push_back(coordSystems.back());
//         }
//         else if (line == "pop")
//         {
//             coordSystems.pop_back();
//         }
//         else if (line == "clear")
//         {
//             screen.graphics.clear({0, 0, 0, 255});
//             screen.clearZbuf();
//         }
//         else
//         {
//             std::cout << "Unrecognized command!" << std::endl;
//         }
//         //std::cout << "top: \n" << coordSystems.back().toString() << std::endl;
//     }
// }

int main()
{
    OBJLoader obj("resources/monkey.obj");

    Screen screen(500, 500);
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);

    RenderObject monkey = obj.toRenderObject("Suzanne");
    monkey.position = {0, 0, -3};
    monkey.rotation = {(15) * M_PI / 180, (-30) * M_PI / 180, (3) * M_PI / 180};

    screen.graphics.renderObject(cam, monkey);
    screen.display();

    // OBJLoader obj("resources/test2.obj");

    // Screen screen(500, 500);
    // Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);

    // RenderObject monkey = obj.toRenderObject("Plane");
    // monkey.position = {0, -.5, -3};
    // monkey.rotation = {(40) * M_PI / 180, (-10) * M_PI / 180, (3) * M_PI / 180};

    // screen.graphics.renderObject(cam, monkey);
    // screen.display();

    // srand(time(NULL));
    // Mat4 edges(0);
    // Mat4 triangles(0);
    // std::vector<Mat4> coordSystems;
    // coordSystems.push_back(Mat4::identity());

    // Screen screen(500, 500);
    // Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);

    // RenderObject obj1;
    // //obj1.addTriangle({0, 0, -3}, {1, 0, -3}, {0.5, 1, -3});
    // //obj1.addSphere({0, 0, 0}, 1, 30, 15);
    // obj1.addTorus({0, 0, 0}, .2, .7, 30, 12);
    // //obj1.addBox({0, 0, 0}, {1, 1, 1});
    // obj1.position = {0, 0, -3};
    // obj1.rotation = {0, (55 + 180) * M_PI / 180, 20 * M_PI / 180};

    // for (uint i = 0; i < obj1.getMesh().size(); i++) {
    //     obj1.getMesh()[i].color = {static_cast<double>((i * 7 + 120)%256), static_cast<double>((i * 3 + 80)%256), static_cast<double>((i * 13 + 130)%256), 255.0};
    // }

    // screen.graphics.renderObject(cam, obj1);
    // screen.display();

    // for (auto & vert : obj1.getMesh()) {
    //     std::cout << vert.pos << std::endl;
    // }

    // RenderObject obj1;
    // // obj1.getMesh().addSphere({0, 0, 0}, 1, 30, 15);
    // // obj1.getMesh().addTorus({0, 0, 0}, .2, .8, 60, 24);
    // obj1.addBox({0, 0, 0}, {1, 1, 1});
    // // std::cout << std::endl;
    // // for (auto & vert : obj1.getMesh()) {
    // //     std::cout << vert.pos << std::endl;
    // // }
    // //std::cout << obj1.getMeshIndices().size() << std::endl;
    // obj1.position = {.2, 1.2, -3};
    // //obj1.rotation = {0, 0.5, 0};

    // //cam.rotation.y = 10 * M_PI / 180;

    // screen.graphics.renderObject(cam, obj1);
    // screen.display();

    //obj1.rotation.x = 25 * M_PI / 180;

    // RenderObject obj2;
    // obj2.getMesh().addSphere({0, 0, 0}, 1, 50, 25);
    // obj2.position = {0, 0, 0.01};

    // for (int i = 0; i < 36; i++)
    // {
    //     double angle = M_PI * 2 / 36 * i;

    //     cam.rotation.y = angle;

    //     screen.graphics.clear({0, 0, 0, 255});
    //     screen.clearZbuf();
    //     screen.graphics.renderObject(cam, obj1);

    //     std::string s = std::to_string(i);
    //     s.insert(s.begin(), 3 - s.size(), '0');

    //     screen.toFileExtension("img" + s + ".png");
    // }

    // int counter = 0;
    // for (double i = 0; i < 2*M_PI * 4; i += (2*M_PI) / 60 * 4) {
    //     screen.graphics.clear({0, 0, 0, 255});
    //     screen.clearZbuf();

    //     obj1.rotation = {i/2, i, i/4};

    //     screen.graphics.renderObject(cam, obj1);

    //     std::string s = std::to_string(counter);
    //     s.insert(s.begin(), 3 - s.size(), '0');

    //     std::cout << "img" + s + ".png" << std::endl;

    //     screen.toFileExtension("img" + s + ".png");
    //     counter++;
    // }
    //parse("script", screen, edges, triangles, coordSystems);

    return 0;
}