#include "directshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"
#include "../materials/phong.h"

DirectShader::DirectShader() :
    hitColor(Vector3D(1, 0, 0))
{
}

DirectShader::DirectShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{
}

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Vector3D L;
    Intersection its;
    Intersection its_light;
    Vector3D wi;
    double distance;
    Vector3D wo = -r.d;

    Vector3D color(0.0, 0.0, 0.0);
    if (Utils::getClosestIntersection(r, objList, its)) {
        for (int j = 0;j < lsList.size();j++) {
            wi = lsList[j]->sampleLightPosition() - its.itsPoint;
            if (its.shape->getMaterial().hasSpecular()) {
                //MIRROR
                Vector3D wr = 2 * dot(its.normal, wo) * its.normal - wo;
                Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
                color = computeColor(reflectionRay, objList, lsList);
            }
            else if (its.shape->getMaterial().hasTransmission()) {
                //TRANSMISSION
                //double n1 = 1.0; //air
                //double n2 = its.shape->getMaterial().getIndexOfRefraction();
                Vector3D normal = its.normal;
                double mu_t = its.shape->getMaterial().getIndexOfRefraction();
                if (dot(wo, normal) < 0) {
                    mu_t = 1 / mu_t;
                    normal = -normal;
                }

                double radicand = 1 - mu_t * mu_t * (1 - dot(normal, wo) * dot(normal, wo));
                if (radicand >= 0) { // NO total internal reflection, compute the refracted ray
                    Vector3D wt = -mu_t * wo + normal * (mu_t * dot(normal, wo) - sqrt(radicand));
                    Ray refracRay = Ray(its.itsPoint, wt, r.depth + 1);
                    color = computeColor(refracRay, objList, lsList);
                }
                else { // Total internal reflection, color = Specular reflection, acts like a mirror
                    Vector3D wr = 2 * dot(normal, wo) * normal - wo;
                    Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
                    color = computeColor(reflectionRay, objList, lsList);
                }
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
                    int N = 256;
                    L = lsList[j]->getIntensity(); //incident light intensity
                    
                    HemisphericalSampler sampler;
					
					double pdf = 1.0 / (2 * 3.14159);
					Ray sampleRay = Ray(its.itsPoint, wj, r.depth + 1);
                    for (int i = 0; i < N; i++) {
                        Vector3D wj = sampler.getSample(its.normal);
                        Vector3D reflectance = its.shape->getMaterial().getReflectance(its.normal, wo, wj);
                        color += ( * reflectance * dot(wj, its.normal)) / pdf;
                    }
                    color = color / N;
                }
            }

        }
        color += its.shape->getMaterial().getDiffuseReflectance() * (0.3, 0.3, 0.3);

    }
    return color;
}
