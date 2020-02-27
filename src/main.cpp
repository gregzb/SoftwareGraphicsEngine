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

int main()
{
    OBJLoader obj("resources/uvtest3.obj");

    Screen screen(500, 500);
    screen.graphics.clear({255, 0, 255, 255});
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);
    //RenderObject monkey = obj.toRenderObject("Plane");
    RenderObject monkey = obj.toRenderObject("Icosphere");
    monkey.setPosition({0, 0, -3});
    monkey.setRotation({(0) * M_PI / 180, (-90) * M_PI / 180, (0) * M_PI / 180});

    screen.graphics.renderObject(cam, monkey);
    screen.display();

    return 0;
}