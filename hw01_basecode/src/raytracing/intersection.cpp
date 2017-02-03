#include <raytracing/intersection.h>

Intersection::Intersection():
    point(glm::vec3(0)),
    normal(glm::vec3(0)),
    t(-1),
    object_hit(nullptr)
{
}

IntersectionEngine::IntersectionEngine()
    : scene(nullptr)
{
}

Intersection IntersectionEngine::GetIntersection(Ray r)
{
    QList<Intersection> intersectionList = GetAllIntersections(r);
    Intersection finalIntersection = Intersection();
    int tMin =float(INFINITY);
    int objectFlag = 0;
    Intersection currentIntersection = Intersection();

    if(intersectionList.size()!=0)
    {
        for(int tempCount = 0;tempCount<intersectionList.size();tempCount++)
        {
            currentIntersection = intersectionList[tempCount];
            if((currentIntersection.t<tMin)&&(currentIntersection.t>0))
            {
                objectFlag = tempCount;
                tMin = currentIntersection.t;
                finalIntersection.normal = currentIntersection.normal;
                finalIntersection.t = currentIntersection.t;
                finalIntersection.object_hit = currentIntersection.object_hit;
                finalIntersection.point = currentIntersection.point;
            }
        }
    }

    return finalIntersection;
}

QList<Intersection> IntersectionEngine::GetAllIntersections(Ray r)
{
    QList<Intersection> intersectionList;
    int objectListSize = scene->objects.size();
    Intersection currentIntersection = Intersection();
//    QString lightName_1 = "Light Source";
//    QString lightName_2 = "Light";

    for(int tempCount = 0;tempCount<objectListSize;tempCount++)
    {
        currentIntersection = scene->objects[tempCount]->GetIntersection(r);

//        if((scene->objects[tempCount]->name == lightName_1)||(scene->objects[tempCount]->name == lightName_2))
//        {
//            continue;
//        }
        if((currentIntersection.t!=-1)&&(currentIntersection.object_hit!=NULL))
        {
            intersectionList.push_back(currentIntersection);
        }
    }

    return intersectionList;
}
