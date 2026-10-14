#include "transmissive.h"

#include <iostream>

Transmissive::Transmissive()
{
}

Transmissive::Transmissive(double indexOfRefraction_) :
    indexOfRefraction(indexOfRefraction_) {
}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {
    return  -1;
};

Vector3D Transmissive::getEmissiveRadiance() const
{
	return Vector3D(0.0);
};

Vector3D Transmissive::getDiffuseReflectance() const
{
	return Vector3D(0.0);
};

double Transmissive::getIndexOfRefraction() const
{
    return indexOfRefraction;
};

