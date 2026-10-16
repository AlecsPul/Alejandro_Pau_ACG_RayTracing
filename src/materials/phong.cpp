#include "phong.h"

#include <iostream>

Phong::Phong()
{ }

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {

    //FILL(...)
    
    Vector3D wr = 2 * dot(n ,wi) * n - wi; //Compute Ideal reflection direction
    Vector3D wo_normal = wo.normalized();
    Vector3D wr_normal = wr.normalized();
    Vector3D reflectance = (rho_d / 3.14159) + ((alpha+2)/(2*3.14159))*Ks * powf(fmax(dot(wr_normal, wo_normal),0), alpha); //Reflectance of a phong material
    return reflectance;

};

double Phong::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Phong::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Phong::getDiffuseReflectance() const
{
    return rho_d;
}

