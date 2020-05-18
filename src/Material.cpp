# include "Material.hpp"
# include "Utils.hpp"
# include <cmath>

std::map<std::string, Material> Material::materials;

Material::Material(std::string name_) : name(name_)
{
}

void Material::setKa(Vec4 const &t)
{
    ka = t;
}
void Material::setKd(Vec4 const &t)
{
    kd = t;
}
void Material::setKs(Vec4 const &t)
{
    ks = t;
}
void Material::setKe(Vec4 const &t)
{
    ke = t;
}
void Material::setNs(double t)
{
    ns = t;
}
void Material::addBumpMap(std::string filename)
{
    bumpMap = PixelGrid<Color>::loadTexture(filename);
    //bumpMap.value().display();
}
void Material::addDiffuseMap(std::string filename)
{
    kdMap = PixelGrid<Color>::loadTexture(filename);
    //kdMap.value().display();
}
void Material::addSpecularMap(std::string filename)
{
    ksMap = PixelGrid<Color>::loadTexture(filename);
    //ksMap.value().display();
}
void Material::addEmissiveMap(std::string filename)
{
    keMap = PixelGrid<Color>::loadTexture(filename);
    //keMap.value().display();
}
void Material::addShininessMap(std::string filename)
{
    nsMap = PixelGrid<Color>::loadTexture(filename);
    //nsMap.value().display();
}

std::string const &Material::getName() const {
    return name;
}

Vec4 const Material::getAmbient(Vec4 const & pos) const
{
    return getDiffuse(pos);
}
Vec4 const Material::getDiffuse(Vec4 const & pos) const
{
    if (kdMap.has_value()) {
        //std::cout << kdMap.value().getHeight() - 1 << std::endl;
        // int v1 = std::lround(pos.getX() * (kdMap.value().getWidth() - 1));
        // int v2 = std::lround(pos.getY() * (kdMap.value().getHeight() - 1));
        // std::cout << v2 << " " << v1 << std::endl;
        // if (v1 != 1024 || v2 != 1023) {
        //     exit(1);
        // }
        // Color c = kdMap.value().read(std::lround(pos.getY() * (kdMap.value().getHeight() - 1)), std::lround(pos.getX() * (kdMap.value().getWidth() - 1)));
        // std::cout << Vec4(c) << std::endl;
        //std::cout << std::lround((pos.getY()) * (kdMap.value().getHeight() - 1)) << " " << std::lround(pos.getX() * (kdMap.value().getWidth() - 1)) << std::endl;
        int row = std::lround(pos[1] * kdMap.value().getHeight());
        row = Utils::clamp(row, 0, kdMap.value().getHeight()-1);
        int col = std::lround(pos[0] * kdMap.value().getWidth());
        col = Utils::clamp(col, 0, kdMap.value().getWidth()-1);
        return kdMap.value().read(row, col);
        //return kdMap.value().read(std::lround(pos.getX() * (kdMap.value().getHeight() - 1)), std::lround((1-pos.getY()) * (kdMap.value().getWidth() - 1)));
    } else {
        return kd;
    }
}
Vec4 const Material::getSpecular(Vec4 const & pos) const
{
    if (ksMap.has_value()) {
        int row = std::lround(pos[1] * ksMap.value().getHeight());
        row = Utils::clamp(row, 0, ksMap.value().getHeight()-1);
        int col = std::lround(pos[0] * ksMap.value().getWidth());
        col = Utils::clamp(col, 0, ksMap.value().getWidth()-1);
        return ksMap.value().read(row, col);
    } else {
        return ks;
    }
}
Vec4 const Material::getEmissive(Vec4 const & pos) const
{
    if (keMap.has_value()) {
        int row = std::lround(pos[1] * keMap.value().getHeight());
        row = Utils::clamp(row, 0, keMap.value().getHeight()-1);
        int col = std::lround(pos[0] * keMap.value().getWidth());
        col = Utils::clamp(col, 0, keMap.value().getWidth()-1);
        return keMap.value().read(row, col);
    } else {
        return ke;
    }
}
Vec4 const Material::getBump(Vec4 const & pos) const
{
    if (bumpMap.has_value()) {
        int row = std::lround(pos[1] * bumpMap.value().getHeight());
        row = Utils::clamp(row, 0, bumpMap.value().getHeight()-1);
        int col = std::lround(pos[0] * bumpMap.value().getWidth());
        col = Utils::clamp(col, 0, bumpMap.value().getWidth()-1);
        return bumpMap.value().read(row, col);
    } else {
        std::cout << "No bump map!" << std::endl;
        exit(0);
        return bumpMap.value().read(0, 0);
    }
}
double Material::getShininess(Vec4 const & pos) const
{
    if (nsMap.has_value()) {
        //Currently in range [0, 255], should probably be remapped to [0, 255], maybe invert, min at 1?
        int row = std::lround(pos[1] * nsMap.value().getHeight());
        row = Utils::clamp(row, 0, nsMap.value().getHeight()-1);
        int col = std::lround(pos[0] * nsMap.value().getWidth());
        col = Utils::clamp(col, 0, nsMap.value().getWidth()-1);
        Color c = nsMap.value().read(row, col);
        return static_cast<double>(c.r) / 255.0;
        //return std::max(c.r, c.g, c.b);
    } else {
        return ns;
    }
}