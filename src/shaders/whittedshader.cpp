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
    
    Vector3D color = (0.0, 0.0, 0.0);
    if (Utils::getClosestIntersection(r, objList, its)) {
            for (int j = 0;j < lsList.size();j++) {
                wi = lsList[j]->sampleLightPosition() - its.itsPoint;
                if (its.shape->getMaterial().hasSpecular()) {
                    //MIRROR

                    Vector3D wr = 2 * dot(its.normal, wi) * its.normal - wi;
                    Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
                    color = computeColor(reflectionRay, objList, lsList);

                }
                else if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
                   
                    Vector3D wi_normal = wi.normalized();
                    Ray ray = Ray();
                    ray.o = its.itsPoint;
                    ray.d = wi_normal;
                    distance = wi.length();
                    ray.maxT = distance;

                    if (Utils::hasIntersection(ray, objList)) {
                        continue;
                    }
                    else {

                        L = lsList[j]->getIntensity(); //incident light intensity
                        Vector3D reflectance = its.shape->getMaterial().getReflectance(its.normal, wo, wi);
                        color += L * reflectance * dot(wi_normal, its.normal);

                    }
                }

            }
            color += its.shape->getMaterial().getDiffuseReflectance() * (0.3, 0.3, 0.3);
        
    }
    return color;
}
