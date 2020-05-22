# pragma once
# include <optional>
# include <string>
# include <map>
# include "Vec4.hpp"
# include "PixelGrid.hpp"
# include "Utils.hpp"

class Material {
    //public:
    Vec4 ka, kd, ks, ke;
    double ns;
    std::optional<PixelGrid<Color>> bumpMap, kdMap, ksMap, keMap, nsMap;
    std::string name;

    public:

    static TextureFiltering TexFiltering;

    static Material defaultMaterial;

    static std::map<std::string, Material> materials;

    Material(std::string name_);
    void setKa(Vec4 const & t);
    void setKd(Vec4 const & t);
    void setKs(Vec4 const & t);
    void setKe(Vec4 const & t);
    void setNs(double t);
    void addBumpMap(std::string filename);
    void addDiffuseMap(std::string filename);
    void addSpecularMap(std::string filename);
    void addEmissiveMap(std::string filename);
    void addShininessMap(std::string filename);

    std::string const & getName() const;
    Vec4 const getAmbient(Vec4 const & pos = {}) const;
    Vec4 const getDiffuse(Vec4 const & pos = {}) const;
    Vec4 const getSpecular(Vec4 const & pos = {}) const;
    Vec4 const getEmissive(Vec4 const & pos = {}) const;
    Vec4 const getBump(Vec4 const & pos = {}) const;
    double getShininess(Vec4 const & pos = {}) const;
};