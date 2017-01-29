#include <scene/geometry/square.h>

Intersection SquarePlane::GetIntersection(Ray r)
{
    //TODO
    Ray transformedRay = r.GetTransformedCopy(transform.invT());
    glm::vec3 rayOrigin = glm::vec3(transformedRay.origin);
    glm::vec3 rayDirection  = glm::vec3(transformedRay.direction);

    glm::vec3 pointS = glm::vec3(-0.5f, 0.5f, 0);
    glm::vec3 normalP = glm::vec3(0,0,1);

    float intersectionT = glm::dot(normalP,(pointS-rayOrigin))/glm::dot(normalP,rayDirection);
    glm::vec3 intersectionP = glm::vec3(transformedRay.origin+intersectionT*transformedRay.direction);

    glm::vec3 centerThree = glm::vec3(0.0);

    glm::vec3 widthVec = glm::vec3(0.0,0.5,0.0);
    float widthVecLengthSqu = glm::length(widthVec)*glm::length(widthVec);
    glm::vec3 heightVec = glm::vec3(0.5,0.0,0.0);
    float  heightVecLengthSqu = glm::length(heightVec)*glm::length(heightVec);

    float dotWidth = std::fabs(glm::dot(intersectionP-centerThree,widthVec));
    float dotHeight = std::fabs(glm::dot(intersectionP-centerThree,heightVec));

    Intersection squareIntersection  = Intersection();

    if((dotWidth<widthVecLengthSqu)&&(dotHeight<heightVecLengthSqu)&&(intersectionT>0))
    {
        glm::vec4 normalPvec4 = glm::vec4(normalP,1);
        glm::vec4 intersectionPvec4 = glm::vec4(intersectionP,1);

        squareIntersection.normal = glm::vec3(transform.invTransT()*normalPvec4);
        squareIntersection.point = glm::vec3(transform.T()*intersectionPvec4);

        squareIntersection.t= intersectionT;

        squareIntersection.object_hit = this;
    }

    return squareIntersection;
}

void SquarePlane::create()
{
    GLuint cub_idx[6];
    glm::vec3 cub_vert_pos[4];
    glm::vec3 cub_vert_nor[4];
    glm::vec3 cub_vert_col[4];

    cub_vert_pos[0] = glm::vec3(-0.5f, 0.5f, 0);  cub_vert_nor[0] = glm::vec3(0, 0, 1); cub_vert_col[0] = material->base_color;
    cub_vert_pos[1] = glm::vec3(-0.5f, -0.5f, 0); cub_vert_nor[1] = glm::vec3(0, 0, 1); cub_vert_col[1] = material->base_color;
    cub_vert_pos[2] = glm::vec3(0.5f, -0.5f, 0);  cub_vert_nor[2] = glm::vec3(0, 0, 1); cub_vert_col[2] = material->base_color;
    cub_vert_pos[3] = glm::vec3(0.5f, 0.5f, 0);   cub_vert_nor[3] = glm::vec3(0, 0, 1); cub_vert_col[3] = material->base_color;

    cub_idx[0] = 0; cub_idx[1] = 1; cub_idx[2] = 2;
    cub_idx[3] = 0; cub_idx[4] = 2; cub_idx[5] = 3;

    count = 6;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, 6 * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos, 4 * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, 4 * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(cub_vert_col, 4 * sizeof(glm::vec3));
}
