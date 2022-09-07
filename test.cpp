#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// !!!glm库中两向量相乘（*）居然是各项分别相乘得到新的向量而不是点乘

#include <iostream>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
float mixValue = 0.4f;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos1 = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront1 = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp1 = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH * 0.5f;
float lastY = SCR_HEIGHT * 0.5f;
float fov = 45.0f;
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
glm::vec3 lightPos(3.2f, 0.0f, 3.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);//无边框窗口

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//还可以这样写？要加载函数写在if语句的条件判断里面？
    {
       std::cout << "Failed to initialize GLAD" << std::endl;
       return -1;
    }



    //unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//把着色器类型以参数形式传递给glCreatShader
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glCompileShader(vertexShader);
    //// check for shader compile errors
    //int success;
    //char infoLog[512];
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}

    //unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
    //unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    //glCompileShader(fragmentShader2);
 
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}

    //unsigned int shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);
    //unsigned int shaderProgram2 = glCreateProgram();
    //glAttachShader(shaderProgram2, vertexShader);
    //glAttachShader(shaderProgram2, fragmentShader2);
    //glLinkProgram(shaderProgram2);


    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //if (!success) {
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    //}
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);
 
    Shader ourShader1("C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\shader.vs", "C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\shader.fs");
    Shader ourShader2("C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\shader2.vs", "C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\shader2.fs");
    Shader lightShader("C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\lightshader.vs", "C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\lightshader.fs");
    // ------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    //unsigned int indices[] = {
    //    0, 1, 3,
    //    1, 2, 3
    //};

    //float Tetrahedron[] = {
    //     1.0f, -1.0f, -1.0f, 0.2f , 0.5f , 0.6f ,
    //    -1.0f, -1.0f,  1.0f, 0.9f , 0.7f , 0.1f ,
    //    -1.0f,  1.0f, -1.0f, 0.8f , 0.0f , 0.8f ,
    //     1.0f, -1.0f, -1.0f, 0.2f , 0.5f , 0.6f ,
    //    -1.0f, -1.0f,  1.0f, 0.9f , 0.7f , 0.1f ,
    //     1.0f,  1.0f,  1.0f, 0.3f , 0.6f , 0.2f ,
    //     1.0f, -1.0f, -1.0f, 0.2f , 0.5f , 0.6f ,
    //    -1.0f,  1.0f, -1.0f, 0.8f , 0.0f , 0.8f ,
    //     1.0f,  1.0f,  1.0f, 0.2f , 0.5f , 0.6f ,
    //    -1.0f, -1.0f,  1.0f, 0.9f , 0.7f , 0.1f ,
    //    -1.0f,  1.0f, -1.0f, 0.8f , 0.0f , 0.8f ,
    //     1.0f,  1.0f,  1.0f, 0.2f , 0.5f , 0.6f ,
    //};

    float secondvertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float alpha =  0;
    float radius = 3.0f;
    glm::vec3 cubePositions[] = {
        glm::vec3(radius* sin(glm::radians(alpha)), radius* cos(glm::radians(alpha)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 36)), radius* cos(glm::radians(alpha + 36)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 72)), radius* cos(glm::radians(alpha + 72)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 108)), radius* cos(glm::radians(alpha + 108)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 144)), radius* cos(glm::radians(alpha + 144)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 180)), radius* cos(glm::radians(alpha + 180)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 216)), radius* cos(glm::radians(alpha + 216)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 252)), radius* cos(glm::radians(alpha + 252)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 288)), radius* cos(glm::radians(alpha + 288)), 0.0f),
        glm::vec3(radius* sin(glm::radians(alpha + 324)), radius* cos(glm::radians(alpha + 324)), 0.0f)
    };

    unsigned int VBOs[2], VAOs[2], EBO;
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(1);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
    // second triangle setup
    // ---------------------
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondvertices), secondvertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    // -----------



    //==========================================================================================================================================
    //灯光VAO绑定
    
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //====================================================================================================================
    
    unsigned int texture1, texture2, texture3, texture4;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\zombie-girl-s.jpg", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\paimeng.jpeg", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    ourShader1.use();
    glUniform1i(glGetUniformLocation(ourShader1.ID, "Texture1"), 0);
    glUniform1i(glGetUniformLocation(ourShader1.ID, "Texture2"), 1);

    //================================================================================

    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data1 = stbi_load("C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\diana.png", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data1);

    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data1 = stbi_load("C:\\Users\\曹治昊\\source\\repos\\OpenGl learning 1st project\\cat.png", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data1);

    ourShader2.use();
    glUniform1i(glGetUniformLocation(ourShader2.ID, "Texture1"), 0);
    glUniform1i(glGetUniformLocation(ourShader2.ID, "Texture2"), 1);

    //======================================================================
    
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        ourShader1.use();
        glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
        unsigned int objectColorloc = glGetUniformLocation(ourShader1.ID, "objectColor");
        glUniform3fv(objectColorloc, 1, glm::value_ptr(objectColor));
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        unsigned int lightColorloc = glGetUniformLocation(ourShader1.ID, "lightColor");
        glUniform3fv(lightColorloc, 1, glm::value_ptr(lightColor));

        //float mixValue = glGetUniformLocation(ourShader1.ID, "mixValue");
        //glUniform1f(mixValue , mixVal);

        ourShader1.setFloat("mixValue", mixValue);

        //glm::mat4 transform = glm::mat4(1.0f);
        //transform = glm::translate(transform, glm::vec3(0.0f, -0.5f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        //transform = glm::translate(transform, glm::vec3(0.0f, 0.5f, 0.0f)); 
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        //glm::mat4 view = glm::mat4(1.0f);
        //float radius = 10.0f;
        //float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        //float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
        //view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));        
        
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        //unsigned int transformLoc = glGetUniformLocation(ourShader1.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        unsigned int modelLoc = glGetUniformLocation(ourShader1.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        unsigned int viewLoc = glGetUniformLocation(ourShader1.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        unsigned int projectionLoc = glGetUniformLocation(ourShader1.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

         //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        // draw first triangle using the data from the first VAO
        //glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f);
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 10.0f * (i + 1);
        //    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    unsigned int model1Loc = glGetUniformLocation(ourShader1.ID, "model");
        //    glUniformMatrix4fv(model1Loc, 1, GL_FALSE, glm::value_ptr(model));
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        //glm::mat4 transform2 = glm::mat4(1.0f);
        //transform2 = glm::translate(transform2, glm::vec3(0.0f, -0.5f, 0.0f));
        //transform2 = glm::rotate(transform2, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, -1.0f));
        //transform2 = glm::translate(transform2, glm::vec3(0.0f, 0.5f, 0.0f));
        //float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        //transform2 = glm::scale(transform2, glm::vec3(scaleAmount));
     
        lightShader.use();

        glm::mat4 lightmodel = glm::mat4(1.0f);
        lightmodel = glm::translate(lightmodel, lightPos);
        lightmodel = glm::scale(lightmodel, glm::vec3(0.3f));
        unsigned int lightmodelLoc = glGetUniformLocation(lightShader.ID, "model");
        glUniformMatrix4fv(lightmodelLoc, 1, GL_FALSE, glm::value_ptr(lightmodel));

        unsigned int lightviewLoc = glGetUniformLocation(lightShader.ID, "view");
        glUniformMatrix4fv(lightviewLoc, 1, GL_FALSE, glm::value_ptr(view));
        unsigned int lightprojectionLoc = glGetUniformLocation(lightShader.ID, "projection");
        glUniformMatrix4fv(lightprojectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //=================================================================================================================================


        //glm::mat4 model1 = glm::mat4(1.0f);
        //model1 = glm::rotate(model, (float)glfwGetTime() * 2, glm::vec3(0.0f, 0.0f, 1.0f));
        //model1 = glm::translate(model1, glm::vec3(0.5f, 0.0f, 0.0f));
        //model1 = glm::scale(model1, glm::vec3(0.3f));
        //model1 = glm::rotate(model1, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view1 = glm::mat4(1.0f);
        view1 = glm::lookAt(cameraPos1, cameraPos1 + cameraFront1, cameraUp1);

        glm::mat4 projection1 = glm::mat4(1.0f);
        projection1 = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture4);

        ourShader2.use();

        ourShader2.setFloat("mixValue", mixValue);

        //unsigned int model1Loc = glGetUniformLocation(ourShader2.ID, "model");
        //glUniformMatrix4fv(model1Loc, 1, GL_FALSE, glm::value_ptr(model1));
        unsigned int view1Loc = glGetUniformLocation(ourShader2.ID, "view");
        glUniformMatrix4fv(view1Loc, 1, GL_FALSE, glm::value_ptr(view1));
        unsigned int projection1Loc = glGetUniformLocation(ourShader2.ID, "projection");
        glUniformMatrix4fv(projection1Loc, 1, GL_FALSE, glm::value_ptr(projection1));

        
        glBindVertexArray(VAOs[1]);

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            float angle = 10.0f * (i + 1);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::rotate(model, (float)glfwGetTime() * 3, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, cubePositions[i]);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            unsigned int model1Loc = glGetUniformLocation(ourShader2.ID, "model");
            glUniformMatrix4fv(model1Loc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &lightVAO);
    //glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f;
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    float cameraSpeed = static_cast<float>(5.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
        cameraPos1 += cameraSpeed * glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
        cameraPos1 -= cameraSpeed * glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0.0f, cameraFront.z), cameraUp)) * cameraSpeed;
        cameraPos1 -= glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0.0f, cameraFront.z), cameraUp1)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0.0f, cameraFront.z), cameraUp)) * cameraSpeed;
        cameraPos1 += glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0.0f, cameraFront.z), cameraUp1)) * cameraSpeed;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);//前两个0代表左下角的位置
}

//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColor;\n"
//"out vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"   ourColor = aColor;\n"
//"}\0";
//
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"in vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(ourColor,1.0);\n"
//"}\n\0";
//
//const char* fragmentShaderSource2 = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
//"}\n\0";

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front = glm::vec3(0.0f);
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    cameraFront1 = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= 5.0f * (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 65.0f)
        fov = 65.0f;
}