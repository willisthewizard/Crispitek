
#include "mesh.h"

mesh::mesh(std::vector<vertexData> vertexData, std::vector<unsigned int> indices)
{
    this->vertices = vertexData;
    this->indices = indices;

    initMesh();
}
void mesh::initMesh()
{
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(vertexData)), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

    //postions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertexData), (void*)0);

    //normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(vertexData), (void*)offsetof(vertexData,normals));

    //texture realated set up

    glBindVertexArray(0);

}
//this is just going to draw the mesh without the textures
void mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES,indices.size(), GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}