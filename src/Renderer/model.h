#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
class model
{
    public:
        std::vector<mesh> meshes;
        std::string filePath;
        model(std::string path);
        void initModel();
        void nodeTraverse(aiNode *node, const aiScene *scene);
        mesh popMeshes(aiMesh *mesh, const aiScene *scene);
        void draw();
};