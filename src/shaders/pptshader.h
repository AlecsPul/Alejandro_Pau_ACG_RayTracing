#ifndef PPTSHADER_H
#define PPTSHADER_H

#include "shader.h"


class Pptshader : public Shader
{
public:
    PptShader();
    PptShader(Vector3D hitColor, Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D hitColor;

};

#endif // AREASHADER_H
