#include "pptshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"
#include "../materials/phong.h"

PptShader::PptShader() :
    hitColor(Vector3D(1, 0, 0))
{
}

PptShader::PptShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{
}

Vector3D PptShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    int maxDepth = 4; // random
    if (r.depth >= maxDepth) {
        return bgColor; // to change, we must return a color shooting a new ray
    }
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
            double pdf = 1.0 / (2 * 3.14159); //since we will only have one light, we can leave it like this  

            HemisphericalSampler sampler = HemisphericalSampler();
            Vector3D wi = sampler.getSample(its.normal);
            Vector3D wi_normalized = wi.normalized();

            Vector3D brdf = its.shape->getMaterial().getReflectance(its.normal, -r.d, wi_normalized);
            if (r.depth < maxDepth) {
                Ray newRay = Ray(its.itsPoint, wi_normalized, r.depth + 1); //maybe not normalized?                    
                color += computeColor(newRay, objList, lsList) * brdf * dot(its.normal, wi_normalized) / pdf;
            }
        }
    }
    return color;
}
