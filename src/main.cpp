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
#include "Scene.hpp"
#include "Light.hpp"

int main()
{
    Material::TexFiltering = TextureFiltering::BILINEAR;

    OBJLoader obj("resources/towerthing.obj");
    Screen screen(600, 600, false);
    //Camera cam(170, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000, false);
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000, true);
    Scene mainScene;

    RenderObject r = obj.toRenderObject("Aset_other__L_ufnscjdga_LOD2");
    r.updateVertexNormals();
    mainScene.addObject("Sphere", r);
    mainScene.getObject("Sphere").setPosition({0, 0, 0});
    mainScene.getObject("Sphere").setRotation({0 * M_PI / 180, -60 * M_PI / 180, 0});

    mainScene.addLight("Ambient", {LightType::Ambient, {0.05, 0.05, 0.05}});
    mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-1, -1, -1}});

    // OBJLoader obj2("resources/skybox.obj");
    // RenderObject skyBox = obj2.toRenderObject("Cube");
    // skyBox.updateVertexNormals();
    // skyBox.setRotation({0, -180 * M_PI / 180, 0});
    // mainScene.setSkybox(skyBox);

    double dist = 2.2;

    cam.setPosition({0, 1.6, dist});
    cam.setLookMode(LookMode::LOOKAT);
    cam.lookAt({0, .8, 0, 0});
    //cam.setRotation({-20 * M_PI / 180, 0* M_PI/180, 0});
    //cam.setRotation({270 * M_PI / 180, 0 * M_PI / 180, 0});

    screen.clear({255, 255, 255, 255});
    mainScene.renderToScreen(cam, screen);

    screen.display();

    // system("mkdir -p ./animation/");
    // system("rm ./animation/*");

    // for (int i = 0; i < 60; i++)
    // {
    //     std::cout << i << std::endl;
    //     // Vec4 rot = mainScene.getObject("Sphere").getRotation();
    //     // rot.setY(2 * M_PI / 60 * i);
    //     // mainScene.getObject("Sphere").setRotation(rot);
    //     // Vec4 rot = cam.getRotation();
    //     // rot.setY(2 * M_PI / 60 * i);
    //     // cam.setRotation(rot);

    //     double x = std::sin(2 * M_PI / 60 * i) * dist;
    //     double y = cam.getPosition()[1];
    //     double z = std::cos(2 * M_PI / 60 * i) * dist;
    //     cam.setPosition({x, y, z});

    //     // std::cout << cam.getPosition() << std::endl;

    //     screen.clear({0, 0, 0, 255});
    //     screen.clear({255, 255, 255, 255});
    //     screen.clearZbuf();

    //     mainScene.renderToScreen(cam, screen);

    //     std::string s = std::to_string(i);
    //     s.insert(s.begin(), 5 - s.size(), '0');

    //     screen.toFileExtension("./animation/img" + s + ".png");
    // }

    // int delay = 4;
    // std::string outputName = "animation.gif";

    // system(("convert -delay " + std::to_string(delay) + " ./animation/* " + outputName).c_str());
    // std::cout << "The animation was rendered into " << outputName << std::endl;
    // system(("animate " + outputName).c_str());

    return 0;
}