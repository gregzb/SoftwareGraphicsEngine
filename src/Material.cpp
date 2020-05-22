#include "Material.hpp"
#include "Utils.hpp"
#include <cmath>

Material Material::defaultMaterial("_");

std::map<std::string, Material> Material::materials;

TextureFiltering Material::TexFiltering = TextureFiltering::BILINEAR;

Material::Material(std::string name_) : name(name_)
{
    if (name == "_") {
        setKd({0.5, 0.5, 0.5});
        setKs({0.5, 0.5, 0.5});
        setNs(50);
    }
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

std::string const &Material::getName() const
{
    return name;
}

Vec4 const Material::getAmbient(Vec4 const &pos) const
{
    return getDiffuse(pos);
}
Vec4 const Material::getDiffuse(Vec4 const &pos) const
{
    if (kdMap.has_value())
    {
        if (Material::TexFiltering == TextureFiltering::CLOSEST)
        {
            Vec4 corrected = kdMap.value().rawRead(pos);
            corrected.set(0, std::pow(corrected[0], 1 / 2.2));
            corrected.set(1, std::pow(corrected[1], 1 / 2.2));
            corrected.set(2, std::pow(corrected[2], 1 / 2.2));
            return corrected;
        }
        else
        {
            Vec4 corrected = kdMap.value().linRead(pos);
            corrected.set(0, std::pow(corrected[0], 1 / 2.2));
            corrected.set(1, std::pow(corrected[1], 1 / 2.2));
            corrected.set(2, std::pow(corrected[2], 1 / 2.2));
            return corrected;
        }
    }
    else
    {
        Vec4 corrected = kd;
        corrected.set(0, std::pow(corrected[0], 1 / 2.2));
        corrected.set(1, std::pow(corrected[1], 1 / 2.2));
        corrected.set(2, std::pow(corrected[2], 1 / 2.2));
        return corrected;
    }
}
Vec4 const Material::getSpecular(Vec4 const &pos) const
{
    if (ksMap.has_value())
    {
        if (Material::TexFiltering == TextureFiltering::CLOSEST)
        {
            return ksMap.value().rawRead(pos);
        }
        else
        {
            return ksMap.value().linRead(pos);
        }
    }
    else
    {
        return ks;
    }
}
Vec4 const Material::getEmissive(Vec4 const &pos) const
{
    if (keMap.has_value())
    {
        if (Material::TexFiltering == TextureFiltering::CLOSEST)
        {
            return keMap.value().rawRead(pos);
        }
        else
        {
            return keMap.value().linRead(pos);
        }
    }
    else
    {
        return ke;
    }
}
Vec4 const Material::getBump(Vec4 const &pos) const
{
    if (bumpMap.has_value())
    {
        if (Material::TexFiltering == TextureFiltering::CLOSEST)
        {
            return bumpMap.value().rawRead(pos);
        }
        else
        {
            return bumpMap.value().linRead(pos);
        }
    }
    else
    {
        std::cout << "No bump map!" << std::endl;
        exit(0);
        return bumpMap.value().read(0, 0);
    }
}
double Material::getShininess(Vec4 const &pos) const
{
    if (nsMap.has_value())
    {
        //Currently in range [0, 255], should probably be remapped to [0, 255], maybe invert, min at 1?

        double temp = 255;

        if (Material::TexFiltering == TextureFiltering::CLOSEST)
        {
            // Color c = nsMap.value().rawRead(pos);
            // return static_cast<double>(c.r);
            Color c = nsMap.value().rawRead(pos);
            temp = temp - static_cast<double>(c.r);
        }
        else
        {
            // Color c = nsMap.value().linRead(pos).toColor();
            // return static_cast<double>(c.r);
            Color c = nsMap.value().linRead(pos).toColor();
            temp = temp - static_cast<double>(c.r);
        }
        return temp;
    }
    else
    {
        return ns;
    }
}