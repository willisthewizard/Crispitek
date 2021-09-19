#include "Renderer.h"

int main()
{
    const char *vertShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    const char *fragShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


    float vertices[] = {
        0.5f,  0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f,  
        -0.5f, -0.5f, 0.0f,  
        -0.5f,  0.5f, 0.0f   
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
    
    glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
   
    

    

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f,0.2f,0.3f,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

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

