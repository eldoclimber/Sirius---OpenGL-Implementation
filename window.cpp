#include "window.h"

Window::Window(int width, int height, const char* title)
    : width(width), height(height), title(title),
    vertexShaderSource("#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0"),
    fragmentShaderSource("#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0"),
    vertices{ -0.5f, -0.5f, 0.0f,
               0.5f, -0.5f, 0.0f,
               0.0f,  0.5f, 0.0f } {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }
}

Window::~Window() {
    glfwTerminate();
}

bool Window::initialize() {
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Create a shader program
    if (!createShaderProgram()) {
        return false;
    }

    return true;
}

void Window::loop() {
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here: clear the screen and draw the triangle
        glClear(GL_COLOR_BUFFER_BIT);

        drawTriangle();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
}


bool Window::createShaderProgram() {
    // Create a vertex shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach the shader source code to the shader object and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if the shader compiled successfully
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }

    // Create a fragment shader object
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach the shader source code to the shader object and compile the shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check if the shader compiled successfully
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }

    // Create a shader program object
    shaderProgram = glCreateProgram();
    // Attach the previously compiled shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if the shaders were linked successfully
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }

    // Delete the shader objects after linking them into the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Window::drawTriangle() {
    // Generate a Vertex Array Object (VAO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO first, then bind and set the VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Use our shader program when we want to render an object
    glUseProgram(shaderProgram);
    // Bind the VAO
    glBindVertexArray(VAO);
    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

