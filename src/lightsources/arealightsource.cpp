#include "arealightsource.h"
#include <random>
namespace
{
    thread_local std::mt19937 generator(std::random_device{}());
    thread_local std::uniform_real_distribution<double> random01(0.0, 1.0);
}

AreaLightSource::AreaLightSource(Square* areaLightsource_) :
    myAreaLightsource(areaLightsource_)
{ }



Vector3D AreaLightSource::getIntensity() const
{
    return myAreaLightsource->getMaterial().getEmissiveRadiance();
}


Vector3D AreaLightSource::sampleLightPosition()   const
{
    //FILL(...)
    // 
    //New Randam Pos inside Area Lightsource
    const double u = random01(generator);
    const double v = random01(generator);

    return myAreaLightsource->corner
        + myAreaLightsource->v1 * u
        + myAreaLightsource->v2 * v;
}

