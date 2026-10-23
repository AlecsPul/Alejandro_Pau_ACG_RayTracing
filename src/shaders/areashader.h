#ifndef AREASHADER_H
#define AREASHADER_H

#include "shader.h"


class AreaShader : public Shader
{
public:
    AreaShader();
    AreaShader(Vector3D hitColor, Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D hitColor;

};

#endif // AREASHADER_H
