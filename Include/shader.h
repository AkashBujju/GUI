#ifndef MY_SHADER_H
#define MY_SHADER_H

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int make_program(std::string v_name, std::string f_name);
void set_vec4f(const unsigned int program, const char* var_name, float f1, float f2, float f3, float f4);
void set_vec3f(const unsigned int program, const char* var_name, float f1, float f2, float f3);
void set_mat4f(const unsigned int program, const char* var_name, glm::mat4 mat);
void set_vec1f(const unsigned int program, const char* var_name, float f);

unsigned int make_program(std::string v_name, std::string f_name)
{
    unsigned int program = -1;

    std::ifstream v_file(v_name), f_file(f_name);
    if(!v_file)
    {
        std::cout << "make_program()-> Could not open file: " << v_name << std::endl;
        return -1;
    }
    if(!f_file)
    {
        std::cout << "make_program()-> Could not open file: " << f_name << std::endl;
        return -1;
    }

    std::stringstream v_stream, f_stream;
    v_stream << v_file.rdbuf();
    f_stream << f_file.rdbuf();

    std::string v_str = v_stream.str();
    std::string f_str = f_stream.str();
    const char* v_code = v_str.c_str();
    const char* f_code = f_str.c_str();

    int success;
    char infoLog[512];

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v_code, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "VertexShader Error: \n" << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f_code, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "FragmentShader Error: \n" << infoLog << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Program Error: \n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void set_vec4f(const unsigned int program, const char* var_name, float f1, float f2, float f3, float f4)
{
    int loc = glGetUniformLocation(program, var_name);
    glUniform4f(loc, f1, f2, f3, f4);
}

void set_vec1f(const unsigned int program, const char* var_name, float f)
{
	int loc = glGetUniformLocation(program, var_name);
	glUniform1f(loc, f);
}

void set_vec3f(const unsigned int program, const char* var_name, float f1, float f2, float f3)
{
	int loc = glGetUniformLocation(program, var_name);
	glUniform3f(loc, f1, f2, f3);
}

void set_mat4f(const unsigned int program, const char* var_name, glm::mat4 mat)
{
	int loc = glGetUniformLocation(program, var_name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

#endif
