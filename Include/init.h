#ifndef INIT_H
#define INIT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* make_window(int scr_width, int scr_height, const char* title);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void init_glfw();
void init_glad();

GLFWwindow* make_window(int scr_width, int scr_height, const char* title)
{
    GLFWwindow *win = glfwCreateWindow(scr_width, scr_height, title, NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    return win;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void init_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void init_glad()
{
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

#endif