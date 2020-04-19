#include <chrono>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = R"END(#version 330 core
layout (location = 0) in vec3 aPos;
out vec2 vertexColor;
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = vec2(aPos.xy);
}
)END";

const char* fragmentShaderSource = R"END(#version 330 core
uniform vec2 timeColor;
out vec4 FragColor;
void main(){
    FragColor = vec4(timeColor, 0.0, 1.0);
}
)END";

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create Windowing Object.
  GLFWwindow* window =
      glfwCreateWindow(800, 600, "OpenGL", NULL, NULL); // ignore last 2 params.
  if (window == nullptr) {
    std::cout << "Failed to create Window." << std::endl;
    glfwTerminate();
    return -1;
  }

  // Tell GLFW to make window to be the main context on the current thread.
  glfwMakeContextCurrent(window);
  // Set callback for window resize.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function.
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
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
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
              << std::endl;
  }
  // Fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
              << std::endl;
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
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
              << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  float vertices[] = {
      0.5f, 0.5f, 0.0f,  // top right
      0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f, 0.0f   // top left
          - 0.75f, -0.75f, 0.0f   // top left
  };
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,  // first Triangle
  };

  unsigned int indices_bottom[] = {  // note that we start from 0!
      1, 2, 3   // second Triangle
  };

  /* VBO
   * -------------------------------------------------------------------------
   * A Vertex Buffer Object (VBO) is a memory buffer in the high speed memory
   * of your video card designed to hold information about vertices.
   * OpenGL has many types of buffer objects and the buffer type of a
   * vertex buffer object is GL_ARRAY_BUFFER.
   */
  //Create a new buffer and bind it to the vbo int handle.
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  // Bind this buffer to be the current/active GL_ARRAY_BUFFER.
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Push data from CPU object "vertices" to current/active GL_ARRAY_BUFFER.
  // The last arg, GL_STATIC_DRAW specifies to the graphics card how
  // to internally manage the given data.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* EBO
   * -------------------------------------------------------------------------
   * A Element Buffer Object (EBO) is a memory buffer to support
   * indexed drawing. The buffer object is of type GL_ELEMENT_ARRAY_BUFFER and
   * stores indices (as unsigned int) of the triangles to be drawn. The
   * indices are in to the VBO. Binding and data pushing semantics are same as
   * VBO.
   */
  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(indices),
               indices,
               GL_STATIC_DRAW);

  unsigned int ebo_bottom;
  glGenBuffers(1, &ebo_bottom);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_bottom);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(indices_bottom),
               indices_bottom,
               GL_STATIC_DRAW);

  /* VAO
 * -------------------------------------------------------------------------
 * A Vertex Array Object (VAO) is an object which contains one or more
 * Vertex Buffer Object and is designed to store the information for a
 * complete rendered object. Each VAO is to be configured separately.
 */
  unsigned int vao[2]; // This int is the handle to the VAOs array.
  glGenVertexArrays(2, vao); // First arg is how many VAOs are to be generated.


  // Setup VAO-0
  glBindVertexArray(vao[0]);
  /* Tell OpenGL how to interpret the VAO elements.*/
  // Find which input is the interpretation for. This looks up the index of
  // the input attribute location using its name.
  unsigned int
      positionAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
  // VertexAttribPointer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(positionAttribLocation);
  glVertexAttribPointer(positionAttribLocation,
                        3,                  // size of each vertex attrib
                        GL_FLOAT,           // type
                        GL_FALSE,           // should this data be normalized?
                        3 * sizeof(float),  // size in bytes of each vertex
                        (void*) 0);        // offset
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  // Release VAO binding.
  glBindVertexArray(0);

  // Setup VAO-1
  glBindVertexArray(vao[1]);
  /* Tell OpenGL how to interpret the VAO elements.*/
  // Find which input is the interpretation for. This looks up the index of
  // the input attribute location using its name.
  // VertexAttribPointer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(positionAttribLocation);
  glVertexAttribPointer(positionAttribLocation,
                        3,                  // size of each vertex attrib
                        GL_FLOAT,           // type
                        GL_FALSE,           // should this data be normalized?
                        3 * sizeof(float),  // size in bytes of each vertex
                        (void*) 0);        // offset
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_bottom);

  // ** Method to bind VAOs is :
  // Bind VAO to active
  // BindBuffer VBO to active ARRAY
  // Enable VBO
  // Interpret VBO pointer
  // Bindbuffer EBO to active ELEMENT_ARRAY
  // Release VAO binding

  // Render loop
  int i = 0;
  int timeColorAttribLocation = glGetUniformLocation(shaderProgram, "timeColor");
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto time = std::chrono::system_clock::now();
    auto seconds = time.time_since_epoch().count() % 256;
    auto timeColor1 = std::sin(static_cast<float>(seconds) / 256.0);
    glUniform2f(timeColorAttribLocation, timeColor1, 0.5);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao[i % 2]);

    // seeing as we only have a single vao there's no need to bind it every
    // time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(window);
    glfwPollEvents();
    ++i;
    std::this_thread::sleep_for(std::chrono_literals::operator""ms(200));
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  glDeleteVertexArrays(2, vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return 0;
}