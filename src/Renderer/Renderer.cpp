#include "Renderer.h"
#include <stb_image.h>

    glm::vec3 camCoords = glm::vec3(0.0f,0.0f,3.0f);
    glm::vec3 camView = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 camDirection = glm::normalize(camCoords - camView);
    glm::vec3 camFront = glm::vec3(0.0f,0.0f,-1.0f);
    glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
    glm::vec3 camRight = glm::normalize(glm::cross(up,camDirection));
    glm::vec3 camUP = glm::cross(camDirection,camRight);
    bool firstMovement = true;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 800.0f / 2.0;
    float lastY = 600.0 /2.0;
int main()
{
    const char *vertShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoords;\n"

    "out vec2 TexCoords;\n"

    "uniform mat4 model;\n"    
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"

    "void main()\n"
    "{\n"
       "TexCoords = aTexCoords;\n"    
       "gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\0";



    const char *fragShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"

    "in vec2 TexCoords;\n"

    "uniform sampler2D texture_diffuse1;\n"

    "void main()\n"
    "{\n"    
        "FragColor = texture(texture_diffuse1, TexCoords);\n"
    "}\0";

   
    
    
    
    //initizlize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create the window
    GLFWwindow* window = glfwCreateWindow(800,600, "Crispitek",NULL,NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  

    if(window == NULL)
    {
        std::cout << "Poopy Stinky" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<< "you arent very Glad are you" << std::endl;
        return -1;
    }

        glViewport(0,0,800,600);
        glfwSetFramebufferSizeCallback(window, framebuffer_resize);

    

    //Vertex Shader
    unsigned int vertShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader,1, &vertShaderSource,NULL);
    glCompileShader(vertShader);

    //Evaluating Vertex Shader
    int sucessful;
    char log[512];
    glGetShaderiv(vertShader,GL_COMPILE_STATUS,&sucessful);
    if(!sucessful)
    {
        glGetShaderInfoLog(vertShader,512,NULL,log);
        std::cout <<"Vertex Shader failed to compile" << log << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    //Fragment Shader
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource,NULL);
    glCompileShader(fragShader);

    //Compiling the Shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertShader);
    glAttachShader(shaderProgram,fragShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&sucessful);
    if(!sucessful)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,log);
    }
    //glUseProgram(shaderProgram);

    //Deleting the shaders now that they have been linked
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    

    glUseProgram(shaderProgram);

    model modelBoi("resources/backpack.obj");
    std::cout << "num Meshes: " << modelBoi.meshes.size() << std::endl;
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f,0.2f,0.3f,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        


        glUseProgram(shaderProgram);

        


        glm::mat4 model = glm::mat4(1.0f);
        
        glm::mat4 projection = glm::mat4(1.0f);

        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        glm::mat4 view = glm::mat4(1.0f);
        const float radius = 10.0f;
        float x = sin(glfwGetTime()) * radius;
        float z = sin(glfwGetTime()) * radius;
        //glm::mat4 view;
        view = glm::lookAt(camCoords,camCoords+camFront,up);
        //setMat4
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        
        //glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f , 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,&view[0][0]);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"projection"), 1, GL_FALSE, &projection[0][0]);
       
       //glm::mat4 model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
       model = glm::scale(model,glm::vec3(1.0f,1.0f,1.0f));
       glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"model"), 1, GL_FALSE, &model[0][0]);

        modelBoi.draw();
        // for(unsigned int i = 0; i < modelBoi.meshes.size(); i++)
        // {
        //     //meshes[i].draw(shader);
        
        //     glBindVertexArray(modelBoi.meshes[i].VAO);
        //     glDrawElements(GL_TRIANGLES,modelBoi.meshes[i].indices.size(), GL_UNSIGNED_INT,0);
        //     glBindVertexArray(0);
        // }
 
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
void framebuffer_resize(GLFWwindow* window,int width, int height)
{
    glViewport(0,0,width,height);
}
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
       glfwSetWindowShouldClose(window,true);
    }
    const float camSpeed = 0.05f;
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
    {
        camCoords += camSpeed * camFront;
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
    {
        camCoords -= camSpeed * camFront;
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
    {
        camCoords -= glm::normalize(glm::cross(camFront,up)) * camSpeed;
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
    {
        camCoords += glm::normalize(glm::cross(camFront,up)) * camSpeed;
    }
}
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if(firstMovement)
    {
        lastX = xPos;
        lastY = yPos;
        firstMovement = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if(pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);
}
