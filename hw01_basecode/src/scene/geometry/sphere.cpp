#include "sphere.h"

#include <iostream>

#include <la.h>

//#define PI 3.1415926535897

static const int SPH_IDX_COUNT = 2280;  // 760 tris * 3
static const int SPH_VERT_COUNT = 382;

Intersection Sphere::GetIntersection(Ray r)
{
    //TODO

    Ray transformedRay = r.GetTransformedCopy(transform.invT());
    glm::vec3 sphereCenter = glm::vec3(0.0);
    float radius = 0.5;
    glm::vec3 R_0 = glm::vec3(transformedRay.origin);
    glm::vec3 R_d = glm::vec3(transformedRay.direction);

    float A = R_d[0]*R_d[0]+R_d[1]*R_d[1]+R_d[2]*R_d[2];
    float B = 2*(R_d[0]*(R_0[0]-sphereCenter[0])+R_d[1]*(R_0[1]-sphereCenter[1])+R_d[2]*(R_0[2]-sphereCenter[2]));
    float C = (R_0[0]-sphereCenter[0])*(R_0[0]-sphereCenter[0])+(R_0[1]-sphereCenter[1])*(R_0[1]-sphereCenter[1])+(R_0[2]-sphereCenter[2])*(R_0[2]-sphereCenter[2])-radius*radius;

    float discriminant = B*B-4*A*C;
    Intersection sphereIntecsection = Intersection();

    if(discriminant>0)
    {
        float t0 = (-B-sqrt(discriminant))/(2*A);
        float t1 = (-B+sqrt(discriminant))/(2*A);

        if(t0>0)
        {
            sphereIntecsection.point = R_0+t0*R_d;
            sphereIntecsection.t = t0;
            sphereIntecsection.object_hit = this;
            sphereIntecsection.normal = glm::normalize(sphereIntecsection.point-sphereCenter);


            glm::vec4 tempPointPvec4 = glm::vec4(sphereIntecsection.point,1);
            sphereIntecsection.point = glm::vec3(transform.T()*tempPointPvec4);

            glm::vec4 tempNormalvec4 = glm::vec4(sphereIntecsection.normal,1);
            sphereIntecsection.normal = glm::normalize(glm::vec3(transform.invTransT()*tempNormalvec4));
        }
        else
        {
            if(t1>0)
            {
                sphereIntecsection.point = R_0+t1*R_d;
                sphereIntecsection.t = t1;
                sphereIntecsection.object_hit = this;
                sphereIntecsection.normal = glm::normalize(sphereIntecsection.point-sphereCenter);


                glm::vec4 tempPointPvec4 = glm::vec4(sphereIntecsection.point,1);
                sphereIntecsection.point = glm::vec3(transform.T()*tempPointPvec4);

                glm::vec4 tempNormalvec4 = glm::vec4(sphereIntecsection.normal,1);
                sphereIntecsection.normal = glm::normalize(glm::vec3(transform.invTransT()*tempNormalvec4));
            }
        }

    }
    else if(discriminant==0)
    {
        float t0 = (-B-sqrt(discriminant))/(2*A);
        if(t0>0)
        {
            sphereIntecsection.point = R_0+t0*R_d;
            sphereIntecsection.t = t0;
            sphereIntecsection.object_hit = this;
            sphereIntecsection.normal = glm::normalize(sphereIntecsection.point-sphereCenter);


            glm::vec4 tempPointPvec4 = glm::vec4(sphereIntecsection.point,1);
            sphereIntecsection.point = glm::vec3(transform.T()*tempPointPvec4);

            glm::vec4 tempNormalvec4 = glm::vec4(sphereIntecsection.normal,1);
            sphereIntecsection.normal = glm::normalize(glm::vec3(transform.invTransT()*tempNormalvec4));
        }
    }

    return sphereIntecsection;
}

// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createSphereVertexPositions(glm::vec3 (&sph_vert_pos)[SPH_VERT_COUNT])
{
    // Create rings of vertices for the non-pole vertices
    // These will fill indices 1 - 380. Indices 0 and 381 will be filled by the two pole vertices.
    glm::vec4 v;
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            v = glm::rotate(glm::mat4(1.0f), j / 20.f * TWO_PI, glm::vec3(0, 1, 0))
                * glm::rotate(glm::mat4(1.0f), -i / 18.0f * PI, glm::vec3(0, 0, 1))
                * glm::vec4(0, 0.5f, 0, 1);
            sph_vert_pos[(i - 1) * 20 + j + 1] = glm::vec3(v);
        }
    }
    // Add the pole vertices
    sph_vert_pos[0] = glm::vec3(0, 0.5f, 0);
    sph_vert_pos[381] = glm::vec3(0, -0.5f, 0);  // 361 - 380 are the vertices for the bottom cap
}


void createSphereVertexNormals(glm::vec3 (&sph_vert_nor)[SPH_VERT_COUNT])
{
    // Unlike a cylinder, a sphere only needs to be one normal per vertex
    // because a sphere does not have sharp edges.
    glm::vec4 v;
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            v = glm::rotate(glm::mat4(1.0f), j / 20.0f * TWO_PI, glm::vec3(0, 1, 0))
                * glm::rotate(glm::mat4(1.0f), -i / 18.0f * PI, glm::vec3(0, 0, 1))
                * glm::vec4(0, 1.0f, 0, 0);
            sph_vert_nor[(i - 1) * 20 + j + 1] = glm::vec3(v);
        }
    }
    // Add the pole normals
    sph_vert_nor[0] = glm::vec3(0, 1.0f, 0);
    sph_vert_nor[381] = glm::vec3(0, -1.0f, 0);
}


void createSphereIndices(GLuint (&sph_idx)[SPH_IDX_COUNT])
{
    int index = 0;
    // Build indices for the top cap (20 tris, indices 0 - 60, up to vertex 20)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 0;
        sph_idx[index + 1] = i + 1;
        sph_idx[index + 2] = i + 2;
        index += 3;
    }
    // Must create the last triangle separately because its indices loop
    sph_idx[57] = 0;
    sph_idx[58] = 20;
    sph_idx[59] = 1;
    index += 3;

    // Build indices for the body vertices
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            sph_idx[index] = (i - 1) * 20 + j + 1;
            sph_idx[index + 1] = (i - 1) * 20 +  j + 2;
            sph_idx[index + 2] = (i - 1) * 20 +  j + 22;
            sph_idx[index + 3] = (i - 1) * 20 +  j + 1;
            sph_idx[index + 4] = (i - 1) * 20 +  j + 22;
            sph_idx[index + 5] = (i - 1) * 20 +  j + 21;
            index += 6;
        }
    }

    // Build indices for the bottom cap (20 tris, indices 2220 - 2279)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 381;
        sph_idx[index + 1] = i + 361;
        sph_idx[index + 2] = i + 362;
        index += 3;
    }
    // Must create the last triangle separately because its indices loop
    sph_idx[2277] = 381;
    sph_idx[2278] = 380;
    sph_idx[2279] = 361;
    index += 3;
}

void Sphere::create()
{
    GLuint sph_idx[SPH_IDX_COUNT];
    glm::vec3 sph_vert_pos[SPH_VERT_COUNT];
    glm::vec3 sph_vert_nor[SPH_VERT_COUNT];
    glm::vec3 sph_vert_col[SPH_VERT_COUNT];

    createSphereVertexPositions(sph_vert_pos);
    createSphereVertexNormals(sph_vert_nor);
    createSphereIndices(sph_idx);
    for (int i = 0; i < SPH_VERT_COUNT; i++) {
        sph_vert_col[i] = material->base_color;
    }

    count = SPH_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(sph_idx, SPH_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(sph_vert_pos, SPH_VERT_COUNT * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(sph_vert_col, SPH_VERT_COUNT * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(sph_vert_nor, SPH_VERT_COUNT * sizeof(glm::vec3));
}
glm::vec2 Sphere::GetUVCoordinates(const glm::vec3 &point) const
{
    float localRadius = std::sqrt(point[0]*point[0]+point[2]*point[2]);
    float loopLength = localRadius*2*PI;
    if((localRadius==0)&&(point[1]==0.5))
    {
        return glm::vec2(point[0]+0.5,1);
    }

    if((localRadius==0)&&(point[1]==-0.5))
    {
        return glm::vec2(point[0]+0.5,0);
    }

    return glm::vec2((point[0]+0.5)/loopLength,point[1]+0.5);
//    return glm::vec2(1.0);
}
