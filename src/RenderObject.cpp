#include "RenderObject.hpp"

RenderObject::RenderObject() : scale(1, 1, 1) {}

Mat4 RenderObject::toWorldSpace() {
    Mat4 copy = mesh;

    Mat4 translateMat = Mat4::translate(position);
    Mat4 rotXMat = Mat4::rotX(rotation.x);
    Mat4 rotYMat = Mat4::rotY(rotation.y);
    Mat4 rotZMat = Mat4::rotZ(rotation.z);
    Mat4 scaleMat = Mat4::scale(scale);

    Mat4 model = translateMat.multiply(rotXMat).multiply(rotYMat).multiply(rotZMat).multiply(scaleMat);
    copy.multiplyMutate(model);

    return copy;
}

Mat4 & RenderObject::getMesh() {
    return mesh;
}