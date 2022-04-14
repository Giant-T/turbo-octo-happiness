#ifndef APP_H
#define APP_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class App {
   public:
    App();
    ~App();
    void run();

   private:
    void render(unsigned int shader_program, unsigned int VAO);
    void process_inputs();
    void compile_shader(unsigned int * shader_program);
    GLFWwindow* m_window;
    const GLFWvidmode* m_mode;
};

#endif  // APP_H
