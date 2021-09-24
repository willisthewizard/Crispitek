#include "Renderer.h"
#include <stb_image.h>
int main()
{
    const char *vertShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos, 1.0f);\n"
    "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\n\0";
    const char *fragShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture1;\n"
    "uniform sampler2D texture2;\n"
    "void main()\n"
    "{\n"
    "FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n"
    "}\n\0";

        
    float vertices[] = {      
        // positions          // texture coords           
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
     -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left ft 
    };
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };
    //initizlize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create the window
    GLFWwindow* window = glfwCreateWindow(800,600, "Crispitek",NULL,NULL);
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

    
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;

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

    //glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);

    

    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //position attributtes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //color 
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    
    //texture position attributtes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
   
    //Loading texture
    unsigned int texture,texture2;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load("resources/container.jpg", &width,&height,&nrChannels,0);
    if(imageData)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Texture failed to load" << std::endl;
    }
    //freeing image from memory
    stbi_image_free(imageData);
    
    //texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    imageData = stbi_load("resources/awesomeface.png",&width,&height,&nrChannels,0);
    if(imageData)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Texture2 failed to load" << std::endl;
    }
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1); 

    //unsigned int transformBox = glGetUniformLocation(shaderProgram,"transform");
    //glUniformMatrix4fv(transformBox,1,GL_FALSE,glm::value_ptr(trans));

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f,0.2f,0.3f,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        //create transformations
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f,-0.5f,0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));


        glUseProgram(shaderProgram);

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(transform));
        glBindVertexArray(VAO);

 
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        //glBindVertexArray(0);

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
        glfwSetWindowShouldClose(window,true);
}

