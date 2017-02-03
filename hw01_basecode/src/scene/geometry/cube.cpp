#include "cube.h"
#include <la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

Intersection Cube::GetIntersection(Ray r)
{
    //TODO
    Ray transformedRay = r.GetTransformedCopy(transform.invT());
    glm::vec3 rayOrigin = transformedRay.origin;
    glm::vec3 rayDirection = transformedRay.direction;

    float tNear = -float(INFINITY);
    float tFar = float(INFINITY);

    float t_0 = tNear;
    float t_1 = tFar;

    Intersection cubeIntersection = Intersection();

    float x_min = -0.5;
    float x_max = 0.5;
    float y_min = -0.5;
    float y_max = 0.5;
    float z_min = -0.5;
    float z_max = 0.5;

    //first we deal with x

    if((rayDirection[0]>=0)&&(rayDirection[0]<FLT_EPSILON))
//    if(rayDirection[0]==0)
    {
        if((transformedRay.origin[1]<y_min)||(transformedRay.origin[1]>y_max)||(transformedRay.origin[2]<z_min)||(transformedRay.origin[2]>z_max))
        {
            return cubeIntersection;

        }
    }
    else
    {
        t_0 = (x_min - rayOrigin[0])/rayDirection[0];
        t_1 = (x_max - rayOrigin[0])/rayDirection[0];

        if(t_0>t_1)
        {
            float tempT = t_0;
            t_0 = t_1;
            t_1 = tempT;
        }

        if((t_0>tNear))
        {
            tNear = t_0;
        }
        if((t_1<tFar))
        {
            tFar = t_1;
        }

    }

    //second we deal with y
    if((rayDirection[1]>=0)&&(rayDirection[1]<FLT_EPSILON))
//    if(rayDirection[1]==0)
    {
        if((transformedRay.origin[0]<x_min)||(transformedRay.origin[0]>x_max)||(transformedRay.origin[2]<z_min)||(transformedRay.origin[2]>z_max))
        {
            return cubeIntersection;

        }
    }
    else
    {
        t_0 = (y_min - rayOrigin[1])/rayDirection[1];
        t_1 = (y_max - rayOrigin[1])/rayDirection[1];

        if(t_0>t_1)
        {
            float tempT = t_0;
            t_0 = t_1;
            t_1 = tempT;
        }

        if((t_0>tNear))
        {
            tNear = t_0;
        }
        if((t_1<tFar))
        {
            tFar = t_1;
        }

    }

    //third we deal with z
    if((rayDirection[2]>=0)&&(rayDirection[2]<FLT_EPSILON))
//    if(rayDirection[2]==0)
    {
        if((transformedRay.origin[0]<x_min)||(transformedRay.origin[0]>x_max)||(transformedRay.origin[1]<y_min)||(transformedRay.origin[1]>y_max))
        {
            return cubeIntersection;

        }
    }
    else
    {
        t_0 = (z_min - rayOrigin[2])/rayDirection[2];
        t_1 = (z_max - rayOrigin[2])/rayDirection[2];

        if(t_0>t_1)
        {
            float tempT = t_0;
            t_0 = t_1;
            t_1 = tempT;
        }

        if((t_0>tNear))
        {
            tNear = t_0;
        }
        if((t_1<tFar))
        {
            tFar = t_1;
        }


    }


    //then we sum up the result
    if(tNear<tFar)
    {
        if(tNear>0)
        {
            glm::vec4 tempPointVec4P = glm::vec4(transformedRay.origin+tNear*transformedRay.direction,1);

            cubeIntersection.point = glm::vec3(transform.T()*tempPointVec4P);
            cubeIntersection.t = tNear;
            cubeIntersection.object_hit = this;

            float pointCoordX = tempPointVec4P[0];
            float pointCoordY = tempPointVec4P[1];
            float pointCoordZ = tempPointVec4P[2];

            float maxCoord  = std::max(fabs(pointCoordX),std::max(fabs(pointCoordY),fabs(pointCoordZ)));

            cubeIntersection.normal = glm::vec3 (0.0);
            if(fabs(tempPointVec4P[0])==maxCoord)
            {
                cubeIntersection.normal[0] = tempPointVec4P[0]/fabs(tempPointVec4P[0]);

            }
            else if(fabs(tempPointVec4P[1]==maxCoord))
            {
                 cubeIntersection.normal[1] = tempPointVec4P[1]/fabs(tempPointVec4P[1]);
            }
            else
            {
                cubeIntersection.normal[2] = tempPointVec4P[2]/fabs(tempPointVec4P[2]);
            }

            glm::vec4 tempNormalVec4 = glm::vec4(cubeIntersection.normal,0);
            tempNormalVec4 = transform.invTransT()*tempNormalVec4;
            cubeIntersection.normal = glm::normalize(glm::vec3(tempNormalVec4));

        }
        else if(tFar>0)
        {
            glm::vec4 tempPointVec4P = glm::vec4(transformedRay.origin+tFar*transformedRay.direction,1);

            cubeIntersection.point = glm::vec3(transform.T()*tempPointVec4P);
            cubeIntersection.t = tFar;
            cubeIntersection.object_hit = this;

            float pointCoordX = tempPointVec4P[0];
            float pointCoordY = tempPointVec4P[1];
            float pointCoordZ = tempPointVec4P[2];

            float maxCoord  = std::max(fabs(pointCoordX),std::max(fabs(pointCoordY),fabs(pointCoordZ)));

            cubeIntersection.normal = glm::vec3 (0.0);
            if(fabs(tempPointVec4P[0])==maxCoord)
            {
                cubeIntersection.normal[0] = tempPointVec4P[0]/fabs(tempPointVec4P[0]);

            }
            else if(fabs(tempPointVec4P[1]==maxCoord))
            {
                 cubeIntersection.normal[1] = tempPointVec4P[1]/fabs(tempPointVec4P[1]);
            }
            else
            {
                cubeIntersection.normal[2] = tempPointVec4P[2]/fabs(tempPointVec4P[2]);
            }

            glm::vec4 tempNormalVec4 = glm::vec4(cubeIntersection.normal,0);
            tempNormalVec4 = transform.invTransT()*tempNormalVec4;
            cubeIntersection.normal = glm::normalize(glm::vec3(tempNormalVec4));
        }
    }

    return cubeIntersection;
}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createCubeVertexPositions(glm::vec3 (&cub_vert_pos)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
}


void createCubeVertexNormals(glm::vec3 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,1);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(1,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(-1,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,-1);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,1,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,-1,0);
    }
}

void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

void Cube::create()
{
    GLuint cub_idx[CUB_IDX_COUNT];
    glm::vec3 cub_vert_pos[CUB_VERT_COUNT];
    glm::vec3 cub_vert_nor[CUB_VERT_COUNT];
    glm::vec3 cub_vert_col[CUB_VERT_COUNT];

    createCubeVertexPositions(cub_vert_pos);
    createCubeVertexNormals(cub_vert_nor);
    createCubeIndices(cub_idx);

    for(int i = 0; i < CUB_VERT_COUNT; i++){
        cub_vert_col[i] = material->base_color;
    }

    count = CUB_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, CUB_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos,CUB_VERT_COUNT * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, CUB_VERT_COUNT * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(cub_vert_col, CUB_VERT_COUNT * sizeof(glm::vec3));

}
glm::vec2 Cube::GetUVCoordinates(const glm::vec3 &point) const
{
    if(fabs(point[0])==0.5)
    {
        return glm::vec2(point[1]+0.5,point[2]+0.5);
    }
    if(fabs(point[1])==0.5)
    {
        return glm::vec2(point[0]+0.5,point[2]+0.5);
    }
    if(fabs(point[2])==0.5)
    {
        return glm::vec2(point[0]+0.5,point[1]+0.5);
    }
//    return glm::vec2(1.0);
}
