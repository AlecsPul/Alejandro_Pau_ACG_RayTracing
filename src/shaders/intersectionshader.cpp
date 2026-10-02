#include "intersectionshader.h"
#include "../core/utils.h"

IntersectionShader::IntersectionShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

IntersectionShader::IntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D IntersectionShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<LightSource*> &lsList) const
{
    /*for (int i = 0; i < objList.size();i++) {
        if (objList.at(i)->rayIntersectP(r)){
			return Vector3D(1, 0, 0); //red)
        }

    }*/

    //THESE TWO ARE THE SAME, SECOND IS A BUILT-IN FUNCTION.
	if (Utils::hasIntersection(r, objList)) {
		return Vector3D(1, 0, 0); //red
	}

   
   
    return bgColor; 
}
