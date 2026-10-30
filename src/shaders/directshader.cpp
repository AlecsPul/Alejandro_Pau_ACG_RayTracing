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
    double distance;
    Vector3D wo = -r.d;

    Vector3D color(0.0, 0.0, 0.0);
    if (Utils::getClosestIntersection(r, objList, its)) {
        
        if (its.shape->getMaterial().hasSpecular()) {
            //MIRROR
            Vector3D wr = 2 * dot(its.normal, wo) * its.normal - wo;
            Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
            color = computeColor(reflectionRay, objList, lsList);
        }
        
        else if (its.shape->getMaterial().hasTransmission()) {
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

        else if (its.shape->getMaterial().isEmissive()) {
            color = its.shape->getMaterial().getEmissiveRadiance();
        }

        else if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
            HemisphericalSampler sampler;
					
			double pdf = 1.0 / (2 * 3.14159);

            Vector3D wj = sampler.getSample(its.normal);
            Vector3D brdf = its.shape->getMaterial().getReflectance(its.normal, wo, wj);
                        
			Ray newRay = Ray(its.itsPoint, wj);
                        
            Intersection y;
            Utils::getClosestIntersection(newRay, objList, y);
                        
            if (y.shape->getMaterial().isEmissive()) {
                Vector3D l_dir = y.shape->getMaterial().getEmissiveRadiance();
                color += (l_dir * brdf * dot(wj, its.normal)) / pdf;
            }
                
        }
        color += its.shape->getMaterial().getDiffuseReflectance() * (0.3, 0.3, 0.3);
    }
    return color;
}
