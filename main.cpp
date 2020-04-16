#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const GLchar* vertexShaderSource = R"END(
#version 330 core
layout (location = 0) in vec3 aPos;
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)END";

const GLchar* fragmentShaderSource = R"END(
out vec4 FragColor;
void main(){
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
}
)END";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Windowing Object.
    GLFWwindow* window = glfwCreateWindow(800, 600,"OpenGL", NULL, NULL); // ignore last 2 params.
    if(window== nullptr){
        std::cout << "Failed to create Window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Tell GLFW to make window to be the main context on the current thread.
    glfwMakeContextCurrent(window);
    // Set callback for window resize.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    // Communicate  viewport size to opengl.
    glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);
    /* Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed to
     * coordinates on your screen. For example, a processed point of location (-0.5,0.5) would
     * (as its final transformation) be mapped to (200,450) in screen coordinates.
     * Note that processed coordinates in OpenGL are between -1 and 1 so we effectively map from
     * the range (-1 to 1) to (0, 800), widthwise left-to-right, (-1, 1) to (0, 600), heightwise top-to-bottom.
     * Pixels are defined at the grid intersections.
     * http://web.cse.ohio-state.edu/~parent.1/classes/581/Lectures/4.2DviewingHandout.pdf
     */

    // Build and compile shaders.
    // Vertex shader.
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Main Render Loop.
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        // Rendering.
        glClearColor(0.5, 0.5, 0.5, 1.0); // << state setter.
        glClear(GL_COLOR_BUFFER_BIT);     // << state user.

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Exiting." << std::endl;
    return 0;
}