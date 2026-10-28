#include "arealightsource.h"
#include <random>
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

    double randomV1 = rand() / RAND_MAX;
    double randomV2 = rand() / RAND_MAX;

    Vector3D randomPoint = myAreaLightsource->corner + myAreaLightsource->v2 * randomV2 + myAreaLightsource->v1 * randomV1;
    
	
    return randomPoint;
}

