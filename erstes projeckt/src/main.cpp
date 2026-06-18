#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb/stb_image.h>

#include "shaderClass.h"
#include "Texture.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Format: x, y, z,  r, g, b,  u, v,  nx, ny, nz
    GLfloat vertices[] = {
        // pos                   color                  texcoord    normal
        -0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f, // Boden
        -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,  0.0f, 5.0f,   0.0f, -1.0f,  0.0f,
         0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,  5.0f, 5.0f,   0.0f, -1.0f,  0.0f,
         0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,  5.0f, 0.0f,   0.0f, -1.0f,  0.0f,

        -0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,  0.0f, 0.0f,  -0.8f,  0.5f,  0.0f, // Links
        -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,  5.0f, 0.0f,  -0.8f,  0.5f,  0.0f,
         0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,  2.5f, 5.0f,  -0.8f,  0.5f,  0.0f,

        -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,  5.0f, 0.0f,   0.0f,  0.5f, -0.8f, // Hinten
         0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,  0.0f, 0.0f,   0.0f,  0.5f, -0.8f,
         0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,  2.5f, 5.0f,   0.0f,  0.5f, -0.8f,

         0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,  0.0f, 0.0f,   0.8f,  0.5f,  0.0f, // Rechts
         0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,  5.0f, 0.0f,   0.8f,  0.5f,  0.0f,
         0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,  2.5f, 5.0f,   0.8f,  0.5f,  0.0f,

         0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,  5.0f, 0.0f,   0.0f,  0.5f,  0.8f, // Vorne
        -0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,  0.0f, 0.0f,   0.0f,  0.5f,  0.8f,
         0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,  2.5f, 5.0f,   0.0f,  0.5f,  0.8f
    };

    GLuint indices[] = {
        0,  1,  2,   0,  2,  3,  // Boden
        4,  6,  5,               // Links
        7,  9,  8,               // Hinten
        10, 12, 11,              // Rechts
        13, 15, 14               // Vorne
    };

    GLfloat lightVertices[] = {
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };

    GLuint lightIndices[] = {
        0, 1, 2,   0, 2, 3,
        0, 4, 7,   0, 7, 3,
        3, 7, 6,   3, 6, 2,
        2, 6, 5,   2, 5, 1,
        1, 5, 4,   1, 4, 0,
        4, 5, 6,   4, 6, 7
    };

    GLFWwindow* window = glfwCreateWindow(width, height, "Erstes OpenGL Projekt", nullptr, nullptr);
    if (!window) {
        std::cerr << "Fenster konnte nicht erstellt werden\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    std::string base = exeDir();
    shader shaderProgram(
        (base + "/res/Shaders/default.vert").c_str(),
        (base + "/res/Shaders/default.frag").c_str()
    );
    shader lightShader(
        (base + "/res/Shaders/light.vert").c_str(),
        (base + "/res/Shaders/light.frag").c_str()
    );

    // Pyramide VAO
    VAO vao;
    vao.Bind();
    VBO vbo(vertices, sizeof(vertices));
    EBO ebo((GLfloat*)indices, sizeof(indices));
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // Licht VAO
    VAO lightVao;
    lightVao.Bind();
    VBO lightVbo(lightVertices, sizeof(lightVertices));
    EBO lightEbo((GLfloat*)lightIndices, sizeof(lightIndices));
    lightVao.LinkAttrib(lightVbo, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);
    lightVao.Unbind();
    lightVbo.Unbind();
    lightEbo.Unbind();

    glm::vec4 lightColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // rot
    glm::vec3 lightPos   = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);

    glm::mat4 pyramidModel = glm::mat4(1.0f);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"),
                lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"),
                lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"),
                lightPos.x, lightPos.y, lightPos.z);

    Texture tex0((base + "/res/textures/brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    tex0.texUnit(shaderProgram, "tex0", 0);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window, deltaTime);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Pyramide zeichnen
        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"),
                    camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Matrix(shaderProgram, "camMatrix");
        tex0.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // Licht-Würfel zeichnen
        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");
        lightVao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    lightVao.Delete();
    lightVbo.Delete();
    lightEbo.Delete();
    tex0.Delete();
    shaderProgram.Delete();
    lightShader.Delete();

    glfwTerminate();
    return 0;
}


// https://youtu.be/45MIykWJ-C4?si=R9R5B6WdfvhY1QoM&t=4698
// cmake --build "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build" --config Release; & "C:\Users\hanne\Cpp\cpp open gl lernen\erstes projeckt\build\Release\ErstesProjekt.exe"
