#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW Initialisierung fehlgeschlagen\n";
        return -1;
    }

    //version open gl sagen
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //alle punkte setzen
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f
    };

    //fenster mit gröse öfnen
    GLFWwindow* window = glfwCreateWindow(800, 600, "Erstes OpenGL Projekt", nullptr, nullptr);
    if (!window) {
        //fehlerbehandlung
        std::cerr << "Fenster konnte nicht erstellt werden\n";
        glfwTerminate();
        return -1;
    }

    //mahlt content
    glfwMakeContextCurrent(window);

    //ladet die farben
    gladLoadGL();

    glViewport(0, 0, 800, 600);

    //vertexshader erstellen
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    { GLint ok; char log[512]; glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok); if (!ok) { glGetShaderInfoLog(vertexShader, 512, NULL, log); std::cerr << "Vertex Shader Fehler:\n" << log; } }
    //fragment shader erstellen
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    { GLint ok; char log[512]; glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok); if (!ok) { glGetShaderInfoLog(fragmentShader, 512, NULL, log); std::cerr << "Fragment Shader Fehler:\n" << log; } }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    { GLint ok; char log[512]; glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok); if (!ok) { glGetProgramInfoLog(shaderProgram, 512, NULL, log); std::cerr << "Shader Program Fehler:\n" << log; } }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //setz lles zusammen
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glClearColor(0.07f, 0.13f, 0.67f, 1.0f );
    //macht buffer
    glClear(GL_COLOR_BUFFER_BIT);
    //mach swap
    glfwSwapBuffers(window);

    //mainloop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.67f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    //loscht alles was es nicht mer braucht
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //schliest das fenster
    glfwTerminate();
    return 0;
}


// https://youtu.be/45MIykWJ-C4?si=AMpocs4vzjL5LyLw&t=1761
// cmake --build "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build" --config Release; & "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build\Release\ErstesProjekt.exe"