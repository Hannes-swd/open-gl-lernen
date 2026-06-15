#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "shaderClass.h"

#include "VAO.h"

#include "EBO.h"




std::string exeDir() {
    char buf[MAX_PATH];
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    std::string path(buf);
    return path.substr(0, path.find_last_of("\\/"));
}

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
        -0.5f,        -0.5f * float(sqrt(3)) / 3,      0.0f, // Lower left corner
         0.5f,        -0.5f * float(sqrt(3)) / 3,      0.0f, // Lower right corner
         0.0f,         0.5f * float(sqrt(3)) * 2 / 3,  0.0f, // Upper corner
        -0.5f / 2,     0.5f * float(sqrt(3)) / 6,      0.0f, // Inner left
         0.5f / 2,     0.5f * float(sqrt(3)) / 6,      0.0f, // Inner right
         0.0f,        -0.5f * float(sqrt(3)) / 3,      0.0f  // Inner down
    };

    GLuint indices[] =
    {
        0, 3, 5, //unten links
        3, 2, 4, //unten rechts
        5, 4, 1  //oben
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

    std::string base = exeDir();
    shader shaderProgram(
        (base + "/res/Shaders/default.vert").c_str(),
        (base + "/res/Shaders/default.frag").c_str()
    );



    VAO vao;
    vao.Bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo((GLfloat*)indices, sizeof(indices));

    vao.LinkVBO(vbo, 0);

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();


    glClearColor(0.07f, 0.13f, 0.67f, 1.0f );
    //macht buffer
    glClear(GL_COLOR_BUFFER_BIT);
    //mach swap
    glfwSwapBuffers(window);

    //mainloop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.67f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        vao.Bind();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    //loscht alles was es nicht mer braucht
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    shaderProgram.Delete();

    //schliest das fenster
    glfwTerminate();
    return 0;
}


// https://youtu.be/45MIykWJ-C4?si=zG7RZyJd8n3l_F8h&t=2278
// cmake --build "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build" --config Release; & "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build\Release\ErstesProjekt.exe"
