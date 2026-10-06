#include "whittedshader.h"
#include "../core/utils.h"

WhittedShader::WhittedShader() :
    hitColor(Vector3D(1, 0, 0))
{
}

WhittedShader::WhittedShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{
}

Vector3D WhittedShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Vector3D L;
    Intersection its;
	Vector3D wi;
	double distance;
       if (Utils::hasIntersection(r, objList)) {
           for (int j = 0;j < lsList.size();++j) {
               wi = lsList[j]->sampleLightPosition() - its.itsPoint;
               distance = sqrt(wi.x * wi.x + wi.y * wi.y + wi.z * wi.z);
               L = (lsList[j]->getIntensity() / (distance * distance)) * (wi); //
           }
       }
    

    return bgColor;
}
