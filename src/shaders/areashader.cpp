#include "areashader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"
#include "../materials/phong.h"

AreaShader::AreaShader() :
    hitColor(Vector3D(1, 0, 0))
{
}

AreaShader::AreaShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{
}

Vector3D AreaShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
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
            int N = 256;
            
			
			 // change to 1/area
            double pdf = 1.0 / lsList.front()->getArea(); //since we will only have one light, we can leave it like this

            for (int i = 0; i < N; i++) {
                
                Vector3D y = lsList.front()->sampleLightPosition();
                Vector3D wi = y - its.itsPoint;
                Vector3D wi_normalized = wi.normalized();

                Vector3D brdf = its.shape->getMaterial().getReflectance(its.normal, wo, wi_normalized);
                
				
				float mod_wi = wi.length();
                float G = (fmax(0, dot(its.normal, wi_normalized)) * fmax(0, dot(lsList.front()->getNormal(), -wi_normalized))) / (mod_wi*mod_wi);

				Ray shadowRay = Ray();
				shadowRay.o = its.itsPoint;
				shadowRay.d = wi_normalized;
				shadowRay.minT = Epsilon;
				shadowRay.maxT = mod_wi-Epsilon;


				int visibility;
                
                if (Utils::hasIntersection(shadowRay, objList)) {
                    visibility = 0;
                }
                else {
                    visibility = 1;
                }
                        
                
                Vector3D l_dir = lsList.front()->getIntensity();
                color += (l_dir * brdf * G * visibility) / pdf;
                    
            }
            color = color / N;
                
        }

		
        
        color += its.shape->getMaterial().getDiffuseReflectance() * 0.2;
    }
    return color;
}
