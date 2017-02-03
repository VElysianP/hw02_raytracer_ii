#include <F:\Course\CIS561 Advanced Computer Graphics\HW2\hw02_raytracer_ii\rendertask.h>

RenderTask::RenderTask(glm::ivec2 min, glm::ivec2 max, Camera* c, Film* f, Integrator* i)
: camera(c), film(f), integrator(i), minPixel(min), maxPixel(max)
{
}

void RenderTask::run()
{
    for(int i = minPixel[0];i<=maxPixel[0];i++)
    {
        for(int j = minPixel[1];j<=maxPixel[1];j++)
        {
             Ray currentRay = camera.Raycast(tempCount1,tempCount2);
             film.pixels[i][j] = integrator->TraceRay(currentRay,depth);

        }

    }
}
