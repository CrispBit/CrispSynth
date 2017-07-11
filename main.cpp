#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <chrono>

#include "Mesh.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "meemerino", sf::Style::Default, sf::ContextSettings(24));
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    window.setActive(true);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int success;
    char infoLog[512];

    const GLuint shaderProgram = glCreateProgram();
    GLchar const* vssource =
            "#version 130\n"
                    "in vec3 aPos;"
                    "in vec2 texCoord;"
                    "in vec3 aNormal;"
                    "in int mID;"
                    "in ivec4 boneIDs;"
                    "in vec4 weights;"
                    ""
                    "out vec2 texCoordV;"
                    ""
                    "const int MAX_BONES = 4;"
                    "const int MAX_MESHES = 100;"
                    ""
                    "uniform mat4 model;"
                    "uniform mat4 view;"
                    "uniform mat4 proj;"
                    "uniform mat4 gBones[MAX_MESHES * MAX_BONES];"
                    ""
                    "void main() {"
                    "   int id = mID * MAX_BONES;"
                    "   mat4 boneTransform = gBones[id + boneIDs[0]] * weights[0];"
                    "   boneTransform += gBones[id + boneIDs[1]] * weights[1];"
                    "   boneTransform += gBones[id + boneIDs[2]] * weights[2];"
                    "   boneTransform += gBones[id + boneIDs[3]] * weights[3];"
                    "   vec4 posL = boneTransform * vec4(aPos, 1.0);"
                    "   gl_Position = proj * view * posL;"
                    "   texCoordV = texCoord;"
                    "}";
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VS, 1, &vssource, NULL);
    glCompileShader(VS);

    glGetShaderiv(VS, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VS, 512, NULL, infoLog);
        std::cout << "rip shader comperino 1\n" << infoLog << std::endl;
    }

    GLchar const* fssource =
            "#version 130\n"
                    "out vec4 FragColor;"
                    ""
                    "in vec2 texCoordV;"
                    ""
                    "uniform sampler2D ourTexture;"
                    ""
                    "void main() {"
                    "   FragColor = texture(ourTexture, texCoordV);"
                    "}";
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS, 1, &fssource, NULL);
    glCompileShader(FS);

    glGetShaderiv(FS, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(FS, 512, NULL, infoLog);
        std::cout << "rip shader comperino 2\n" << infoLog << std::endl;
    }

    glAttachShader(shaderProgram, VS);
    glAttachShader(shaderProgram, FS);

    glBindAttribLocation(shaderProgram, 0, "aPos");
    glBindAttribLocation(shaderProgram, 1, "texCoord");
    glBindAttribLocation(shaderProgram, 2, "aNormal");
    glBindAttribLocation(shaderProgram, 3, "mID");
    glBindAttribLocation(shaderProgram, 4, "boneIDs");
    glBindAttribLocation(shaderProgram, 5, "weights");

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(VS);
    glDeleteShader(FS);

    glUseProgram(shaderProgram);

    auto t_start = std::chrono::high_resolution_clock::now();

    Mesh object;
    object.loadMesh("assets/Spider_3.fbx");

    GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
    glm::mat4 trans;
    auto t_now = std::chrono::high_resolution_clock::now();

    glm::mat4 view = glm::lookAt(
            glm::vec3(4.0f, 4.0f, 4.0f),
            glm::vec3(3.0f, 3.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
    );
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    float lookDeg = 100;

    glm::mat4 proj = glm::perspective(glm::radians(lookDeg), 800.0f / 600.0f, 1.0f, 10.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    bool running = true;
    bool active = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            do {
                if (event.type == sf::Event::Closed) {
                    // end the program
                    running = false;
                } else if (event.type == sf::Event::Resized) {
                    // adjust the viewport when the window is resized
                    glViewport(0, 0, event.size.width, event.size.height);
                    proj = glm::perspective(glm::radians(lookDeg), (float) event.size.width / (float) event.size.height, 1.0f, 10.0f);
                    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
                } else if (event.type == sf::Event::LostFocus) {
                        active = false;
                } else if (event.type == sf::Event::GainedFocus) {
                    active = true;
                }
            } while (!active && window.waitEvent(event));
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        trans = glm::rotate(
                trans,
                time * glm::radians(.05f),
                glm::vec3(0.0f, 0.0f, 1.0f)
        );
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

        std::vector<std::vector<glm::mat4>> Transforms;
        object.boneTransform(time, Transforms);
        for (unsigned int i = 0; i < Transforms.size(); ++i) {
            for (unsigned int j = 0; j < Transforms[i].size(); ++j) {
                const std::string name = "gBones[" + std::to_string(i * 4 + j) + "]"; // every transform is for a different bone
                GLint boneTransform = glGetUniformLocation(shaderProgram, name.c_str());
                Transforms[i][j] = glm::transpose(Transforms[i][j]);
                glUniformMatrix4fv(boneTransform, 1, GL_TRUE, glm::value_ptr(Transforms[i][j]));
            }
        }

        object.draw();
        window.display();
    }

    window.close();
    return 0;
}
