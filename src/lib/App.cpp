#include "App.h"

#include <iostream>

// Vertex shader
const char* vertex_shader_code =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader
const char* fragment_shader_code =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

App::App() {
    if (!glfwInit()) {
        throw std::exception("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a fullscreen window
    m_window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);

    if (!m_window) {
        glfwTerminate();
        throw std::exception("Failed to create GLFW window");
    }
    // make the window's context current
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::exception("Failed to initialize GLAD");
    }
}

App::~App() { glfwTerminate(); }

void App::run() {
    unsigned int shader_program;
    compile_shader(&shader_program);

    float vertices[]{-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, };
    // float vertices[]{0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                    //  -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};
    unsigned int indices[]{0, 1, 3, 1, 2, 3};

    unsigned int VAO;  // vertex array object
    unsigned int VBO;  // Vertex buffer object
    unsigned int EBO;  // Element buffer object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);  // bind the VAO

    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);  // unbind the VAO

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(m_window)) {
        // process inputs
        process_inputs();

        // Render here
        render(shader_program, VAO);

        // Swap front and back buffers if not the screen will flicker
        glfwSwapBuffers(m_window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);
}

void App::render(unsigned int shader_program, unsigned int VAO) {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void App::process_inputs() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
    if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void App::compile_shader(unsigned int* shader_program) {
    // Compiling the fragment shader
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);

    // check for success
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERRO::SHADER::VERTEX:::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Compiling fragment shader
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);

    // check for success
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERRO::SHADER::FRAGMENT:::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Linking the program
    *shader_program = glCreateProgram();
    glAttachShader(*shader_program, vertex_shader);
    glAttachShader(*shader_program, fragment_shader);
    glLinkProgram(*shader_program);

    // check for success
    glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*shader_program, 512, NULL, infoLog);
        std::cout << "ERRO::SHADER::PROGRAM:::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete the shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
