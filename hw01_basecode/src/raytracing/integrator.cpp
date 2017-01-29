#include <raytracing/integrator.h>

Integrator::Integrator():
    max_depth(5), scene(nullptr), intersection_engine(nullptr)
{
}

//Basic ray trace
glm::vec3 Integrator::TraceRay(Ray r, unsigned int depth)
{
    QList<Geometry*> sceneLights =  scene->lights;
    float T_Threshold = float(pow(10,-4));

    Intersection currentIntersection = intersection_engine->GetIntersection(r);

    glm::vec3 intersectionPoint = currentIntersection.point+T_Threshold*currentIntersection.normal;
    Geometry* objectHit = currentIntersection.object_hit;
    Material* objectMaterial = nullptr;
    glm::vec3 averageColor = glm::vec3(0.0);
    glm::vec3 currentLight = glm::vec3(0.0);

    for(int tempCount=0;tempCount<sceneLights.size();tempCount++)
    {
        if(objectHit!=nullptr)
        {
             objectMaterial=objectHit->material;
             currentLight = sceneLights[tempCount]->transform.position() - intersectionPoint;
             glm::vec3 currentNormalizedLight = glm::normalize(sceneLights[tempCount]->transform.position() - intersectionPoint);
             if(ShadowTesting(intersectionPoint,currentLight)==true)
             {
                 averageColor = averageColor+glm::vec3(0.0);
             }
             else
             {
                 averageColor = averageColor + objectMaterial->EvaluateReflectedEnergy(currentIntersection,currentNormalizedLight,currentNormalizedLight);
             }
        }
    }

    averageColor = glm::vec3(averageColor[0]/sceneLights.size(),averageColor[1]/sceneLights.size(),averageColor[2]/sceneLights.size());
    return averageColor;
}

void Integrator::SetDepth(unsigned int depth)
{
    max_depth = depth;
}

bool Integrator::ShadowTesting(glm::vec3 intersectionPoint, glm::vec3 currentLight)
{
    Ray shadowRay = Ray(intersectionPoint, currentLight);

    Intersection newIntersection = intersection_engine->GetIntersection(shadowRay);

    if(newIntersection.object_hit==nullptr)
    {//which means that the ray does not hit any object
        return false;
    }
    else
    {
        if((newIntersection.t<0)||(newIntersection.t>1))
        {
            return false;
        }
        else
        {
            return true;//which means the ray will be blocked
        }
    }
}
