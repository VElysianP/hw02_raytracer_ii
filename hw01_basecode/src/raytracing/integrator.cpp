#include <raytracing/integrator.h>

Integrator::Integrator():
    max_depth(5), scene(nullptr), intersection_engine(nullptr)
{
}

//Basic ray trace
glm::vec3 Integrator::TraceRay(Ray r, unsigned int depth)
{
    if(depth>max_depth)
    {
        return glm::vec3(0.0);
    }
    else
    {
        QList<Geometry*> sceneLights =  scene->lights;
        float T_Threshold = float(pow(10,-4));

        Intersection currentIntersection = intersection_engine->GetIntersection(r);

        glm::vec3 intersectionPoint = currentIntersection.point+T_Threshold*currentIntersection.normal;
        Geometry* objectHit = currentIntersection.object_hit;
        Material* objectMaterial = nullptr;
        glm::vec3 averageColor = glm::vec3(0.0);
        glm::vec3 currentLight = glm::vec3(0.0);
        glm::vec3 shadowColor = glm::vec3(1.0);

        for(int tempCount=0;tempCount<sceneLights.size();tempCount++)
        {
            if(objectHit!=nullptr)
            {
                 objectMaterial=objectHit->material;
                 currentLight = sceneLights[tempCount]->transform.position() - intersectionPoint;
                 glm::vec3 currentNormalizedLight = glm::normalize(sceneLights[tempCount]->transform.position() - intersectionPoint);


                     averageColor = averageColor + objectMaterial->EvaluateReflectedEnergy(currentIntersection,currentNormalizedLight,currentNormalizedLight,r,depth,*this);
                     averageColor = sceneLights[tempCount]->material->base_color*averageColor;
                     shadowColor = shadowColor*ShadowTesting(intersectionPoint,currentLight);

            }
        }

        averageColor = glm::vec3(averageColor[0]/sceneLights.size(),averageColor[1]/sceneLights.size(),averageColor[2]/sceneLights.size());
        averageColor = shadowColor*averageColor;
        return glm::clamp(averageColor,0.0f,1.0f);

    }


}

void Integrator::SetDepth(unsigned int depth)
{
    max_depth = depth;
}

//bool Integrator::ShadowTesting(glm::vec3 intersectionPoint, glm::vec3 currentLight)
//{
//    Ray shadowRay = Ray(intersectionPoint, currentLight);

//    Intersection newIntersection = intersection_engine->GetIntersection(shadowRay);

//    if(newIntersection.object_hit==nullptr)
//    {//which means that the ray does not hit any object
//        return false;
//    }
//    else
//    {
//        if((newIntersection.t<0)||(newIntersection.t>1))
//        {
//            return false;
//        }
//        else
//        {
//            return true;//which means the ray will be blocked
//        }
//    }
//}

glm::vec3 Integrator::ShadowTesting(glm::vec3 intersectionPoint, glm::vec3 currentLight)
{
    Ray shadowRay = Ray(intersectionPoint,currentLight);
    QList<Intersection> allIntersections = intersection_engine->GetAllIntersections(shadowRay);
    QList<Geometry*> allIntersectionObjects;
    QList<Geometry*> allIntersectionsBeforeLight;

    glm::vec3 allColors = glm::vec3(1.0);


    for(int tempCount =0;tempCount <allIntersections.size();tempCount++)
    {
        allIntersectionObjects.push_back(allIntersections[tempCount].object_hit);
    }

    float lightIntersectionT;

    if(allIntersectionObjects.size()==1)
    {

            return allIntersectionObjects[0]->material->base_color;

    }

    for(int tempCount1 = 0;tempCount1<allIntersectionObjects.size();tempCount1++)
    {
        if((allIntersectionObjects[tempCount1]->material->name=="Light")||(allIntersectionObjects[tempCount1]->material->name=="Light Source"))
        {

            lightIntersectionT = allIntersections[tempCount1].t;

        }
    }
    boolean transparentFlag = false;//false means all the objects are transparent, true means that there being any object that is not transparent

    for(int tempCount2 = 0;tempCount2<allIntersectionObjects.size();tempCount2++)
    {
        if(allIntersections[tempCount2].t<lightIntersectionT)
        {
            allIntersectionsBeforeLight.push_back(allIntersections[tempCount2].object_hit);

            if((allIntersections[tempCount2].object_hit->material->refract_idx_in==0)||(allIntersections[tempCount2].object_hit->material->refract_idx_out==0))
            {
                transparentFlag = true;
            }
        }
    }

    if(transparentFlag==true)//true means that there being any object that is not transparent
    {
        return glm::vec3(0.0);
    }
    else
    {
        glm::vec3 textureColor = glm::vec3(1.0);

        for(int tempCount3 = 0;tempCount3<allIntersectionsBeforeLight.size();tempCount3++)
        {

            allColors = allColors * allIntersectionsBeforeLight[tempCount3]->material->base_color;
            if(allIntersectionsBeforeLight[tempCount3]->material->texture!=nullptr)
            {

                glm::vec2 textureColor2D = allIntersectionsBeforeLight[tempCount3]->GetUVCoordinates(allIntersectionsBeforeLight[tempCount3]->GetIntersection(shadowRay).point);
                textureColor = allIntersectionsBeforeLight[tempCount3]->material->GetImageColor(textureColor2D,allIntersectionsBeforeLight[tempCount3]->material->texture);
                allColors = allColors*textureColor;
            }

        }
        return glm::clamp(allColors,0.0f,1.0f);
    }


}
