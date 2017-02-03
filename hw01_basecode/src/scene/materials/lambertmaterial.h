#pragma once
#include <scene/materials/material.h>

class LambertMaterial : public Material
{
public:
    LambertMaterial();
    LambertMaterial(const glm::vec3 &color);

    // The outgoing ray is the direction along which the light we are computing is being reflected.
    // The incoming ray is the direction from which light is being received by the point of intersection.
    glm::vec3 EvaluateReflectedEnergy(const Intersection &isx,  glm::vec3 &outgoing_ray, const glm::vec3 &incoming_ray,Ray cameraRay,unsigned int depth,Integrator currentIntegrator);
};
