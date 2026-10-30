#ifndef NEESHADER_H
#define NEESHADER_H

#include "shader.h"


class NeeShader : public Shader
{
public:
    NeeShader();
    NeeShader(Vector3D hitColor, Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D hitColor;

};

#endif // NEESHADER_H
