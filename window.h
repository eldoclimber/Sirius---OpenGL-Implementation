#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h> // include GLAD, a loader for OpenGL functions
#include <GLFW/glfw3.h> // include GLFW, a library for creating windows and contexts
#include <iostream> // include the standard I/O library

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool initialize();
    void loop();

private:
    const char* title;
    int width;
    int height;
    GLFWwindow* window;

    const char* vertexShaderSource;
    const char* fragmentShaderSource;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
    float vertices[9];

    bool createShaderProgram();
    void drawTriangle();
};

#endif //WINDOW_H
