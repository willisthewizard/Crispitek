#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader.h"

class mesh {
public:
        struct vertexData
        {

            glm::vec3 vertices; 
            glm::vec3 normals;
            //textureCoords are presently a work in progress
            //glm::vec2 textureCoords;
        };
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        std::vector <vertexData> vertices;
        std::vector<unsigned int> indices; 
        mesh(std::vector<vertexData>, std::vector<unsigned int> indices);
        void initMesh();
        void draw();
};