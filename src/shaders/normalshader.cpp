#include "normalshader.h"
#include "../core/utils.h"

NormalShader::NormalShader() :
    hitColor(Vector3D(1, 0, 0))
{
}

NormalShader::NormalShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{
}

Vector3D NormalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    
    //THESE TWO ARE THE SAME, SECOND IS A BUILT-IN FUNCTION.
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
		Vector3D n = its.normal.normalized(); 
        return (n+(1.0,1.0,1.0))/2.0;
    }



    return bgColor;
}
