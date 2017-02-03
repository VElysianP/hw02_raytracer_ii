#include <F:\Course\CIS561 Advanced Computer Graphics\HW2\hw02_raytracer_ii\rendertask.h>


void RenderTask::run()
{
    for(int i = minPixel[0];i<=maxPixel[0];i++)
    {
        for(int j = minPixel[1];j<=maxPixel[1];j++)
        {
             Ray currentRay = camera->Raycast(i,j);

             film->pixels[i][j] = integrator->TraceRay(currentRay,1);

        }

    }
}
