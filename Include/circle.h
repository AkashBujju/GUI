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

struct CheckBox {
	float org_x = 0;
	float org_y = 0;
	float radius = 0.5f;
	float inc_angle = 1.0f;

	glm::vec3 inner_color;
	glm::vec3 outer_color;
	bool fill_inner = true;

	unsigned int vao, vbo;
	unsigned int vao_2, vbo_2;
	unsigned int program = -1;
	unsigned int program_2 = -1;
	unsigned int num_triangles = 0;
	unsigned int num_lines = 0;

	void init(float org_x, float org_y, float radius, float inc_angle);
	void draw();
};

void CheckBox::init(float org_x, float org_y, float radius, float inc_angle)
{
	this->org_x = org_x;
	this->org_y = org_y;
	this->radius = radius;
	this->inc_angle = inc_angle;

	inner_color = glm::vec3(0.5f, 0.0f, 0.0f);
	outer_color = glm::vec3(0.2f, 0.0f, 0.0f);

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
	delete [] vertices;

	// Outer circle
	vec.clear();	
	angle = 0;
	float f_rad = radius + radius * 0.02f;
	for(; angle <= 360; angle += inc_angle)
	{
		float x = org_x + f_rad * cos(glm::radians(angle));
		float y = org_y + f_rad * sin(glm::radians(angle));

		vec.push_back(x);
		vec.push_back(y);
		vec.push_back(0.0f);
	}

	num_lines = 360.0f / inc_angle;
	_sz = vec.size();
	vertices = new float[_sz];
	for(unsigned int i = 0; i < vec.size(); i++)
		vertices[i] = vec[i];
	
	program_2 = make_program("C:\\Users\\Akash\\Documents\\GitHub\\GUI\\Include\\circle_vertex", "C:\\Users\\Akash\\Documents\\GitHub\\GUI\\Include\\circle_fragment");

	glGenVertexArrays(1, &vao_2);
	glGenBuffers(1, &vbo_2);
	glBindVertexArray(vao_2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * _sz, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	delete [] vertices;
}

void CheckBox::draw()
{
	unsigned int loc = -1;

	if(fill_inner)
	{
		glUseProgram(program);
		loc = glGetUniformLocation(program, "color");
		glUniform4f(loc, inner_color.x, inner_color.y, inner_color.z, 1.0f);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, num_triangles);
	}

	glUseProgram(program_2);
	loc = glGetUniformLocation(program_2, "color");
	glUniform4f(loc, outer_color.x, outer_color.y, outer_color.z, 1.0f);
	glBindVertexArray(vao_2);
	glDrawArrays(GL_LINE_LOOP, 0, num_lines);
}

#endif
