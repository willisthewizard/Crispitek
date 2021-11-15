#include "model.h"
#include <string>
model::model(std::string path)
{
    this->filePath = path;
    initModel();
}

void model::initModel()
{
    Assimp::Importer import;
    
    const aiScene *scene = import.ReadFile(this->filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    //should put some kind of error checker
    
    nodeTraverse(scene->mRootNode,scene);
}

void model::draw()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw();
        std::cout << "The VAO: " << meshes[i].VAO << std::endl;
    }
}
mesh model::popMeshes(aiMesh *aimesh, const aiScene *scene)
{
    std::vector<mesh::vertexData> vertices;
    std::vector<unsigned int> indices; 
    
    for(unsigned int i = 0; i < aimesh->mNumVertices; i++)
    {
        mesh::vertexData data;
        data.vertices.x = aimesh->mVertices[i].x;
        data.vertices.y = aimesh->mVertices[i].y;
        data.vertices.z = aimesh->mVertices[i].z;

        if(aimesh->HasNormals())
        {
            //std::cout << "normals detected" << std::endl;
            data.normals.x = aimesh->mNormals[i].x;
            data.normals.y = aimesh->mNormals[i].y;
            data.normals.z = aimesh->mNormals[i].z;
        }
        vertices.push_back(data);
    }
    for(unsigned int i = 0; i< aimesh->mNumFaces; i++)
    {
        aiFace face = aimesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //invoke a mesh and then return it
    //std::cout << "returning a mesh with num vertices " << vertices.size() << std::endl;
    //std::cout << "returning a mesh with numn indices " << indices.size() << std::endl;
    return mesh(vertices,indices);
    
}
void model::nodeTraverse(aiNode *node, const aiScene *scene)
{
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            //std::cout << "i = : " << i << std::endl;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(popMeshes(mesh,scene));
        }
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            nodeTraverse(node->mChildren[i],scene);
        }
}