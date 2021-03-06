#include <scene/materials/phongmaterial.h>
#include <raytracing/integrator.h>

PhongMaterial::PhongMaterial():
    PhongMaterial(glm::vec3(0.5f, 0.5f, 0.5f))
{}

PhongMaterial::PhongMaterial(const glm::vec3 &color):
    Material(color),
    specular_power(10)
{}
glm::vec3 PhongMaterial::EvaluateReflectedEnergy(const Intersection &isx,  glm::vec3 &outgoing_ray, const glm::vec3 &incoming_ray,Ray cameraRay,unsigned int depth,Integrator currentIntegrator)
{
    glm::vec3 intersectionPoint = isx.point;
    glm::vec3 intersectionNormal = isx.normal;
    glm::vec3 cameraDirection = cameraRay.direction;
    glm::vec3 finalColor = glm::vec3(0.0);
    glm::vec3 LambertColor = base_color*glm::clamp(glm::dot(intersectionNormal, incoming_ray),float(0),float(1));
    glm::vec3 baseColor = glm::vec3(0.0);
    glm::vec3 specularColor = glm::vec3(0.0);
    glm::vec3 refractionColor = glm::vec3(0.0);
    glm::vec3 intersectionPointLocal = glm::vec3(isx.object_hit->transform.invT()*glm::vec4(intersectionPoint,1.0));

    glm::vec2 textureColor2D = isx.object_hit->GetUVCoordinates(intersectionPointLocal);
    glm::vec3 newBaseColor = isx.object_hit->material->GetImageColor(textureColor2D,texture);




    float T_Threshold = float(pow(10,-4));

//
        glm::vec3 reflectionLight = glm::reflect(incoming_ray,intersectionNormal);
        glm::vec3 phongColor = std::pow(glm::clamp(glm::dot(reflectionLight,cameraDirection),0.0f,1.0f),specular_power)*base_color;
        outgoing_ray = reflectionLight;
        baseColor = glm::clamp(LambertColor+phongColor,0.0f,1.0f);


        if(isx.object_hit->material->texture!=nullptr)
        {
            LambertColor = newBaseColor * glm::clamp(glm::dot(intersectionNormal, incoming_ray),float(0),float(1));;
            baseColor = glm::clamp(LambertColor+phongColor,0.0f,1.0f);
        }

//        if(texture!=nullptr)
//        {


//           textureColor = GetImageColor(isx.object_hit->GetUVCoordinates(intersectionPoint),texture);
//           baseColor = baseColor*textureColor;

//        }

        if((reflectivity>0)&&(reflectivity<=1))
        {
//
        glm::vec3 reflectionRay = glm::reflect(cameraDirection,intersectionNormal);
        Ray newCameraRay = Ray(intersectionPoint+T_Threshold*intersectionNormal,reflectionRay);
        specularColor = currentIntegrator.TraceRay(newCameraRay,depth+1);
        specularColor = glm::vec3(specularColor[0]*baseColor[0],specularColor[1]*baseColor[1],specularColor[2]*baseColor[2]);
        //finalColor = (baseColor)*(1-reflectivity)+reflectivity * specularColor;
    }

    float dotProductNI = glm::dot(intersectionNormal,cameraDirection);
    Ray newRefractionCameraRay = Ray(glm::vec3(0.0),glm::vec3(0.0));
    if((refract_idx_in!=0)||(refract_idx_out!=0))
    {
        if(dotProductNI>0)//go out
        {
            float SnellEta12 = refract_idx_in/refract_idx_out;
            glm::vec3 refractionNormal = -intersectionNormal;
            dotProductNI = glm::dot(refractionNormal,cameraDirection);
            float testRefraction = 1-SnellEta12*SnellEta12*(1-dotProductNI*dotProductNI);

            if(SnellEta12>1)
            {
                if(testRefraction>=0)//no total refraction
                {
                    glm::vec3 refreactionRayT = (-SnellEta12*dotProductNI-std::sqrt(testRefraction))*refractionNormal+SnellEta12*cameraDirection;
                    newRefractionCameraRay = Ray(intersectionPoint-T_Threshold*refractionNormal,refreactionRayT);
                    refractionColor = currentIntegrator.TraceRay(newRefractionCameraRay,depth+1);
                    finalColor = refractionColor*(1-reflectivity)+reflectivity*specularColor;
                    return glm::clamp(finalColor,0.0f,1.0f);
                }
                else
                {
                    glm::vec3 totalReflectionDirection = glm::reflect(cameraDirection,refractionNormal);
                    newRefractionCameraRay = Ray(intersectionPoint+T_Threshold*refractionNormal,totalReflectionDirection);
                    glm::vec3 secondReflection = currentIntegrator.TraceRay(newRefractionCameraRay,depth+1);

                    finalColor = secondReflection*(1-reflectivity)+specularColor*reflectivity;
                    return glm::clamp(finalColor,0.0f,1.0f);
                }
            }
            else
            {
                glm::vec3 refreactionRayT = (-SnellEta12*dotProductNI-std::sqrt(testRefraction))*refractionNormal+SnellEta12*cameraDirection;
                newRefractionCameraRay = Ray(intersectionPoint-T_Threshold*refractionNormal,refreactionRayT);
                refractionColor = currentIntegrator.TraceRay(newRefractionCameraRay,depth+1);
                finalColor = refractionColor*(1-reflectivity)+reflectivity*specularColor;
                return glm::clamp(finalColor,0.0f,1.0f);
            }

        }
        else//go inside the object
        {
            float SnellEta12 = refract_idx_out/refract_idx_in;
            float testRefraction = 1-SnellEta12*SnellEta12*(1-dotProductNI*dotProductNI);


            if(SnellEta12>1)
            {
                if(testRefraction>=0)
                {
                    glm::vec3 refreactionRayT = (-SnellEta12*dotProductNI-std::sqrt(testRefraction))*intersectionNormal+SnellEta12*cameraDirection;
                    newRefractionCameraRay = Ray(intersectionPoint-T_Threshold*intersectionNormal,refreactionRayT);
                    refractionColor = currentIntegrator.TraceRay(newRefractionCameraRay,depth+1);
                    finalColor = refractionColor*(1-reflectivity)+reflectivity*specularColor;
                    return glm::clamp(base_color*finalColor,0.0f,1.0f);
                }
                else
                {
                    glm::vec3 totalReflectionDirection = glm::reflect(cameraDirection,intersectionNormal);
                    newRefractionCameraRay = Ray(intersectionPoint+T_Threshold*intersectionNormal,totalReflectionDirection);
                    glm::vec3 secondReflection = currentIntegrator.TraceRay(newRefractionCameraRay,depth+1);

                    finalColor = secondReflection*(1-reflectivity)+specularColor*reflectivity;
                    return glm::clamp(base_color*finalColor,0.0f,1.0f);
                }

            }
            else
            {
                glm::vec3 refreactionRayT = (-SnellEta12*dotProductNI-std::sqrt(testRefraction))*intersectionNormal+SnellEta12*cameraDirection;
                newRefractionCameraRay = Ray(intersectionPoint-T_Threshold*intersectionNormal,refreactionRayT);
                refractionColor = currentIntegrator.TraceRay(newRefractionCameraRay,depth+1);
                finalColor = refractionColor*(1-reflectivity)+reflectivity*specularColor;
                return glm::clamp(base_color*finalColor,0.0f,1.0f);
            }

        }

    }
    else
    {
        return glm::clamp(baseColor*(1-reflectivity)+reflectivity * specularColor,0.0f,1.0f);
    }


}
