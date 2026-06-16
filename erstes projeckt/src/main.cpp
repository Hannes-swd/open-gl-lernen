#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <stb/stb_image.h>

#include "shaderClass.h"
#include "Texture.h"
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

    // Vertices coordinates
    GLfloat vertices[] =
    {
        //   COORDINATES           /      COLORS      //
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  // Lower left corner
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,  // Upper left corner
         0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,  // Upper right corner
         0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f,  // Lower right corner
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 2, 1, // Upper triangle
        0, 3, 2  // Lower triangle
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

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    std::string texPath = base + "/res/textures/pop_cat.png";
    Texture tex0(texPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    tex0.texUnit(shaderProgram, "tex0", 0);

    //mainloop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.67f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        tex0.Bind();
        vao.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    //loscht alles was es nicht mer braucht
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    tex0.Delete();
    shaderProgram.Delete();

    //schliest das fenster
    glfwTerminate();
    return 0;
}


// https://youtu.be/45MIykWJ-C4?si=iawSPr9ziujaBDXc&t=3390
// cmake --build "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build" --config Release; & "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build\Release\ErstesProjekt.exe"
