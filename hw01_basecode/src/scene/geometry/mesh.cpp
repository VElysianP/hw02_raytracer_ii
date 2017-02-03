#include <scene/geometry/mesh.h>
#include <la.h>
#include <tinyobj/tiny_obj_loader.h>
#include <iostream>

Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3):
    Triangle(p1, p2, p3, glm::vec3(1), glm::vec3(1), glm::vec3(1), glm::vec2(0), glm::vec2(0), glm::vec2(0))
{
    for(int i = 0; i < 3; i++)
    {
        normals[i] = plane_normal;
    }
}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3):
    Triangle(p1, p2, p3, n1, n2, n3, glm::vec2(0), glm::vec2(0), glm::vec2(0))
{}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3)
{
    plane_normal = glm::normalize(glm::cross(p2 - p1, p3 - p2));
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    normals[0] = n1;
    normals[1] = n2;
    normals[2] = n3;
    uvs[0] = t1;
    uvs[1] = t2;
    uvs[2] = t3;
}
glm::vec2 Triangle::GetUVCoordinates(const glm::vec3 &point) const
{
    glm::vec3 edge1 = points[1]-points[0];
    glm::vec3 edge2 = points[2]-points[0];

    glm::vec3 edge_1 = point - points[0];
    glm::vec3 edge_2 = point - points[1];
    glm::vec3 edge_3 = point - points[2];

    float totalArea = 0.5*glm::cross(edge1,edge2).length();
    float area1 = 0.5*glm::cross(edge_2,edge_3).length();
    float area2 = 0.5*glm::cross(edge_1,edge_3).length();
    float area3 = 0.5*glm::cross(edge_1,edge_2).length();

    float ratio1 = area1/totalArea;
    float ratio2 = area2/totalArea;
    float ratio3 = area3/totalArea;

    return uvs[0]*ratio1+uvs[1]*ratio2+uvs[2]*ratio3;

}

//Returns the interpolation of the triangle's three normals based on the point inside the triangle that is given.
glm::vec3 Triangle::GetNormal(const glm::vec3 &position)
{
    float d0 = glm::distance(position, points[0]);
    float d1 = glm::distance(position, points[1]);
    float d2 = glm::distance(position, points[2]);
    return (normals[0] * d0 + normals[1] * d1 + normals[2] * d2)/(d0+d1+d2);
}

glm::vec4 Triangle::GetNormal(const glm::vec4 &position)
{
    glm::vec3 result = GetNormal(glm::vec3(position));
    return glm::vec4(result, 0);
}

//The ray in this function is not transformed because it was *already* transformed in Mesh::GetIntersection
Intersection Triangle::GetIntersection(Ray r)
{
    //TODO

    Ray transformedRay = r.GetTransformedCopy(transform.invT());
    glm::vec3 rayOrigin = glm::vec3(transformedRay.origin);
    glm::vec3 rayDirection  = glm::vec3(transformedRay.direction);

    //float intersectionT = glm::dot(normalP,(pointS-rayOrigin))/glm::dot(normalP,rayDirection);

    glm::vec3 E_1 = glm::vec3(points[1]-points[0]);
    glm::vec3 E_2 = glm::vec3(points[2]-points[0]);
    glm::vec3 TVector = rayOrigin - points[0];

    glm::mat3 tempOne = glm::mat3(-rayDirection,E_1,E_2);
    glm::vec3 intersectionResult = glm::inverse(tempOne)*TVector;

    Intersection triIntersection = Intersection();
    if((intersectionResult[1]>=0)&&(intersectionResult[2]>=0)&&(intersectionResult[1]+intersectionResult[2]<=1)&&(intersectionResult[0]>=0))
    {
        glm::vec4 tempPoint = glm::vec4((transformedRay.origin)+intersectionResult[0]*glm::vec3(transformedRay.direction),1);
        triIntersection.point = glm::vec3(transform.T()*tempPoint);
        glm::vec4 tempNormal = glm::vec4(plane_normal,0);
        triIntersection.normal = glm::vec3(transform.invTransT()*tempNormal);
        triIntersection.t = intersectionResult[0];

        triIntersection.object_hit = this;
    }
    return triIntersection;
}

Intersection Mesh::GetIntersection(Ray r)
{
    //TODO
    int totalFaceCount = faces.size();
    Intersection meshIntersection = Intersection();
    Intersection finalMeshIntersection = Intersection();

    Ray transformedRay = r.GetTransformedCopy(transform.invT());

    float minIntersection = float(INFINITY);
    int IntersectionFlag = 0;
    for(int tempCount =0; tempCount<totalFaceCount;tempCount++)
    {
        meshIntersection = faces[tempCount]->GetIntersection(transformedRay);
        if((meshIntersection.t<minIntersection)&&(meshIntersection.t>0))
        {
            IntersectionFlag = tempCount;
            minIntersection = meshIntersection.t;
        }
    }

    if(!(minIntersection==float(INFINITY))||!(minIntersection==-1.0))
    {
       glm::vec4 tempPoint = glm::vec4(faces[IntersectionFlag]->GetIntersection(transformedRay).point,1);
       finalMeshIntersection.point = glm::vec3(transform.T()*tempPoint);

       glm::vec4 tempNormal = glm::vec4(faces[IntersectionFlag]->GetIntersection(transformedRay).normal,0);
       finalMeshIntersection.normal = glm::vec3(transform.invTransT()*tempNormal);

       finalMeshIntersection.object_hit = faces[IntersectionFlag]->GetIntersection(transformedRay).object_hit;

       finalMeshIntersection.t = minIntersection;
    }


    return finalMeshIntersection;
}

void Mesh::SetMaterial(Material *m)
{
    this->material = m;
    for(Triangle *t : faces)
    {
        t->SetMaterial(m);
    }
}


void Mesh::LoadOBJ(const QStringRef &filename, const QStringRef &local_path)
{
    QString filepath = local_path.toString(); filepath.append(filename);
    std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
    std::string errors = tinyobj::LoadObj(shapes, materials, filepath.toStdString().c_str());
    std::cout << errors << std::endl;
    if(errors.size() == 0)
    {
        //Read the information from the vector of shape_ts
        for(unsigned int i = 0; i < shapes.size(); i++)
        {
            std::vector<float> &positions = shapes[i].mesh.positions;
            std::vector<float> &normals = shapes[i].mesh.normals;
            std::vector<float> &uvs = shapes[i].mesh.texcoords;
            std::vector<unsigned int> &indices = shapes[i].mesh.indices;
            for(unsigned int j = 0; j < indices.size(); j += 3)
            {
                glm::vec3 p1(positions[indices[j]*3], positions[indices[j]*3+1], positions[indices[j]*3+2]);
                glm::vec3 p2(positions[indices[j+1]*3], positions[indices[j+1]*3+1], positions[indices[j+1]*3+2]);
                glm::vec3 p3(positions[indices[j+2]*3], positions[indices[j+2]*3+1], positions[indices[j+2]*3+2]);

                Triangle* t = new Triangle(p1, p2, p3);
                if(normals.size() > 0)
                {
                    glm::vec3 n1(normals[indices[j]*3], normals[indices[j]*3+1], normals[indices[j]*3+2]);
                    glm::vec3 n2(normals[indices[j+1]*3], normals[indices[j+1]*3+1], normals[indices[j+1]*3+2]);
                    glm::vec3 n3(normals[indices[j+2]*3], normals[indices[j+2]*3+1], normals[indices[j+2]*3+2]);
                    t->normals[0] = n1;
                    t->normals[1] = n2;
                    t->normals[2] = n3;
                }
                if(uvs.size() > 0)
                {
                    glm::vec2 t1(uvs[indices[j]*2], uvs[indices[j]*2+1]);
                    glm::vec2 t2(uvs[indices[j+1]*2], uvs[indices[j+1]*2+1]);
                    glm::vec2 t3(uvs[indices[j+2]*2], uvs[indices[j+2]*2+1]);
                    t->uvs[0] = t1;
                    t->uvs[1] = t2;
                    t->uvs[2] = t3;
                }
                this->faces.append(t);
            }
        }
        std::cout << "" << std::endl;
    }
    else
    {
        //An error loading the OBJ occurred!
        std::cout << errors << std::endl;
    }
}

void Mesh::create(){
    //Count the number of vertices for each face so we can get a count for the entire mesh
        std::vector<glm::vec3> vert_pos;
        std::vector<glm::vec3> vert_nor;
        std::vector<glm::vec3> vert_col;
        std::vector<GLuint> vert_idx;

        unsigned int index = 0;

        for(int i = 0; i < faces.size(); i++){
            Triangle* tri = faces[i];
            vert_pos.push_back(tri->points[0]); vert_nor.push_back(tri->normals[0]); vert_col.push_back(tri->material->base_color);
            vert_pos.push_back(tri->points[1]); vert_nor.push_back(tri->normals[1]); vert_col.push_back(tri->material->base_color);
            vert_pos.push_back(tri->points[2]); vert_nor.push_back(tri->normals[2]); vert_col.push_back(tri->material->base_color);
            vert_idx.push_back(index++);vert_idx.push_back(index++);vert_idx.push_back(index++);
        }

        count = vert_idx.size();
        int vert_count = vert_pos.size();

        bufIdx.create();
        bufIdx.bind();
        bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufIdx.allocate(vert_idx.data(), count * sizeof(GLuint));

        bufPos.create();
        bufPos.bind();
        bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufPos.allocate(vert_pos.data(), vert_count * sizeof(glm::vec3));

        bufCol.create();
        bufCol.bind();
        bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufCol.allocate(vert_col.data(), vert_count * sizeof(glm::vec3));

        bufNor.create();
        bufNor.bind();
        bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufNor.allocate(vert_nor.data(), vert_count * sizeof(glm::vec3));
}

//This does nothing because individual triangles are not rendered with OpenGL; they are rendered all together in their Mesh.
void Triangle::create(){}


glm::vec2 Mesh::GetUVCoordinates(const glm::vec3 &point) const
{
    glm::vec3 edge1 = glm::vec3(0.0);
    glm::vec3 edge2 = glm::vec3(0.0);

    for(int tempCount =0 ;tempCount<faces.size();tempCount++)
    {
        edge1 = faces[tempCount]->points[1]-faces[tempCount]->points[0];
        edge2 = faces[tempCount]->points[2]-faces[tempCount]->points[0];

        glm::vec3 edge_1 = point - faces[tempCount]->points[0];
        glm::vec3 edge_2 = point - faces[tempCount]->points[1];
        glm::vec3 edge_3 = point - faces[tempCount]->points[2];

        float totalArea = 0.5*glm::cross(edge1,edge2).length();
        float area1 = 0.5*glm::cross(edge_2,edge_3).length();
        float area2 = 0.5*glm::cross(edge_1,edge_3).length();
        float area3 = 0.5*glm::cross(edge_1,edge_2).length();

        if(!(totalArea<(area1+area2+area3)))
        {
            Triangle intersectTriangle = Triangle(faces[tempCount]->points[0],faces[tempCount]->points[1],faces[tempCount]->points[2]);
           return intersectTriangle.GetUVCoordinates(point);
        }
    }
//    return glm::vec2(1.0);
}
