#include <scene/materials/lambertmaterial.h>

LambertMaterial::LambertMaterial():Material()
{}

LambertMaterial::LambertMaterial(const glm::vec3 &color):Material(color)
{}

glm::vec3 LambertMaterial::EvaluateReflectedEnergy(const Intersection &isx, const glm::vec3 &outgoing_ray, const glm::vec3 &incoming_ray)
{
   glm::vec3 surfaceColor = base_color;

    glm::vec3 intersectionNormal = isx.normal;

    surfaceColor = surfaceColor*glm::clamp(glm::dot(intersectionNormal, incoming_ray),float(0),float(1));

    return surfaceColor;

}
