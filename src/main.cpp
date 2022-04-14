#include <iostream>

#include "App.h"

int main(int, char**) {
    std::cout << "Initiating GLFW" << std::endl;

    App app;
    app.run();

    return 0;
}