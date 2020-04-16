#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

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

    // GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function:
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

    // Set callback for window resize.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Main Render Loop.
    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Exiting." << std::endl;
    return 0;
}