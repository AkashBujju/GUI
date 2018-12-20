#ifndef MY_CIRCLE_H
#define MY_CIRCLE_H

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <shader.h>
#include <init.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

struct Circle {
	float org_x = 0;
	float org_y = 0;
	float radius = 0.5f;
	float inc_angle = 1.0f;

	glm::vec3 color;

	unsigned int vao, vbo;
	unsigned int program = -1;
	unsigned int num_triangles = 0;

	void init(float org_x, float org_y, float radius, float inc_angle);
	void draw();
};

void Circle::init(float org_x, float org_y, float radius, float inc_angle)
{
	this->org_x = org_x;
	this->org_y = org_y;
	this->radius = radius;
	this->inc_angle = inc_angle;

	color = glm::vec3(0.5f, 0.0f, 0.0f);

	std::vector<float> vec = { 
		 org_x, org_y, 0.0f,
	};

	float angle = 0;
	for(; angle <= 360; angle += inc_angle)
	{
		float x = org_x + radius * cos(glm::radians(angle));
		float y = org_y + radius * sin(glm::radians(angle));

		vec.push_back(x);
		vec.push_back(y);
		vec.push_back(0.0f);
	}

	num_triangles = 360.0f / inc_angle + 2;
	unsigned int _sz = vec.size();

	float *vertices = new float[_sz];
	for(unsigned int i = 0; i < vec.size(); i++)
		vertices[i] = vec[i];

	program = make_program("C:\\Users\\Akash\\Documents\\GitHub\\GUI\\Include\\circle_vertex", "C:\\Users\\Akash\\Documents\\GitHub\\GUI\\Include\\circle_fragment");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * _sz, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// delete [] vertices;
}

void Circle::draw()
{
	glUseProgram(program);

	unsigned int loc = glGetUniformLocation(program, "color");
	glUniform4f(loc, color.x, color.y, color.z, 1.0f);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, num_triangles);
}

#endif
