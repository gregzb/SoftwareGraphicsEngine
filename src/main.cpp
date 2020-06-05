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

    OBJLoader obj("resources/testscene.obj");
    Screen screen(500, 500, true);
    Camera cam(40, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000, true);
    Scene mainScene;

    auto objects = obj.toRenderObjects();
    for (auto & object : objects) {
        object.second.updateVertexNormals();
        mainScene.addObject(object.first, object.second);
    }

    mainScene.addLight("Point1", {LightType::Point, {}, Vec4(1.2, 0.9, 0.9) * 10, Vec4(1.2, 0.9, 0.9) * 10, {10.57, 2.5, 3.7}});
    mainScene.addLight("Point2", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 7.5, Vec4(1.3, 0.9, 0.9) * 7.5, {-1.1, 2.4, 3.78}});
    mainScene.addLight("Point3", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 12, Vec4(1.3, 0.9, 0.9) * 12, {-11.1, 2.3, 1.7}});
    mainScene.addLight("Point4", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 10, Vec4(1.3, 0.9, 0.9) * 10, {3.7, 2.45, -4.15}});
    mainScene.addLight("Point5", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 5, Vec4(1.3, 0.9, 0.9) * 5, {-6.4, 2.7, -5.2}});
    mainScene.addLight("Point6", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 7, Vec4(1.3, 0.9, 0.9) * 7, {17, 2.35, -3.75}});
    mainScene.addLight("Directional", {LightType::Directional, {}, Vec4(0.2, 0.2, 0.4)*0.67, Vec4(0.2, 0.2, 0.4)*0.67, {-0.8, -0.7, -1.7}});

    OBJLoader obj2("resources/skybox.obj");
    RenderObject skyBox = obj2.toRenderObject("Cube");
    skyBox.updateVertexNormals();
    skyBox.setRotation({0, (-24 + 90) * M_PI / 180, 0});
    mainScene.setSkybox(skyBox);

    cam.setPosition({25.952, 2.8147, -3.3683});
    cam.setLookMode(LookMode::EULER);
    cam.setRotation({-2 * M_PI / 180, 104 * M_PI / 180, 0 * M_PI / 180});

    screen.clear({255, 255, 255, 255});
    screen.clearZbuf();

    mainScene.renderToScreen(cam, screen);
    screen.display();
    std::cout << "Writing output to output1.png" << std::endl;
    screen.toFileExtension("output1.png");


    mainScene.clear();

    OBJLoader otherObj("resources/towerthing.obj");

    auto newObjects = otherObj.toRenderObjects();
    for (auto & object : newObjects) {
        object.second.updateVertexNormals();
        mainScene.addObject(object.first, object.second);
    }

    mainScene.addLight("Ambient", {LightType::Ambient, {0.1, 0.1, 0.1}});
    mainScene.addLight("Point1", {LightType::Point, {}, Vec4(1.4, 0.7, 0.7) * 2.25, Vec4(1.4, 0.7, 0.7) * 2.25, {0.6, 1, 0.3}});
    mainScene.addLight("Point2", {LightType::Point, {}, Vec4(0.4, 0.4, 1.4) * 1.5, Vec4(0.4, 0.4, 1.4) * 1.5, {-0.3, 0.6, 0.5}});
    //mainScene.addLight("Point3", {LightType::Point, {}, Vec4(1, 1, 0.15) * 2, Vec4(1, 1, 0.15) * 2, {0, 3, 0}});

    cam.setPosition({0, 1.5, 3.2});
    cam.setLookMode(LookMode::LOOKAT);
    cam.lookAt({0, 0.675, -1});

    screen.clear({255, 255, 255, 255});
    screen.clearZbuf();

    mainScene.renderToScreen(cam, screen);
    screen.display();
    screen.toFileExtension("output2.png");


    int res;

    res = system("mkdir -p ./animation/");
    res = system("rm ./animation/*");

    int frames = 60;

    for (int i = 0; i < frames; i++)
    {
        std::cout << i << " / " << frames - 1 << std::endl;
        Vec4 rot = mainScene.getObject("Aset_other__L_ufnscjdga_LOD2").getRotation();
        rot.setY(2 * M_PI / 60 * i);
        mainScene.getObject("Aset_other__L_ufnscjdga_LOD2").setRotation(rot);

        screen.clear({0, 0, 0, 255});
        screen.clear({255, 255, 255, 255});
        screen.clearZbuf();

        mainScene.renderToScreen(cam, screen);

        std::string s = std::to_string(i);
        s.insert(s.begin(), 5 - s.size(), '0');

        screen.toFileExtension("./animation/img" + s + ".png");
    }

    int delay = 4;
    std::string outputName = "animation.gif";

    res = system(("convert -delay " + std::to_string(delay) + " ./animation/* " + outputName).c_str());
    std::cout << "The animation was rendered into " << outputName << std::endl;
    res = system(("animate " + outputName).c_str());

    return 0;
}