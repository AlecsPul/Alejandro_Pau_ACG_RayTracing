#include "whittedshader.h"
#include "../core/utils.h"

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
 
    for (int i = 0;i < objList.size();++i) {
        for (int j = 0;j < lsList.size();++j) {
            lsList[j]
        }
    }

    return bgColor;
}
