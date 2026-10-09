#include "whittedshader.h"
#include "../core/utils.h"
#include "../materials/phong.h"

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
    Intersection its_light;
	Vector3D wi;
	double distance;
    Vector3D wo = -r.d;
    Vector3D result;
   
    if (Utils::getClosestIntersection(r, objList, its)) {
        for (int j = 0;j < lsList.size();j++) {

            wi = lsList[j]->sampleLightPosition() - its.itsPoint;
            Vector3D wi_normal = wi.normalized();
            Ray ray = Ray();
            ray.o = its.itsPoint;
            ray.d = wi.normalized();
            distance = wi.length();
            ray.maxT = distance;

            if (Utils::hasIntersection(ray, objList)) {
                continue;
            }
            else {
            
                L = lsList[j]->getIntensity(); //incident light intensity
                Vector3D reflectance = its.shape->getMaterial().getReflectance(its.normal, wo.normalized(), wi_normal);
                result += L * reflectance * dot(wi_normal, its.normal);
            }
        }
    }
    return its.shape->getMaterial().getDiffuseReflectance()* (0.1, 0.1, 0.1) + result;
}
