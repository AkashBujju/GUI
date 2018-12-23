#ifndef MY_RECT_H
#define MY_RECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static float rect_vertices[] = {
	+1.0f, +1.0f, 0.0f,
	+1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	-1.0f, +1.0f, 0.0f
};

static unsigned int rect_indices[] = {
	0, 1, 3,
	1, 2, 3
};

struct BorderRect
{
	private:
		unsigned int vao, vbo, program = -1;
		glm::mat4 model;

	public:
		glm::vec3 pos;
		glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		float x_scale = 0.1f;
		float y_scale = 0.1f;

		void init();
		void draw();
};

struct Rect
{
	private:
		unsigned int vao, vbo, ebo;
		unsigned int program = -1;
		glm::mat4 model;
		const float delay_push_time = 0.2f;
		float current_push_start_time = 0.0f;
		bool should_push = false;
		int count = 0;

		float tmp_x_scale = 0.0;
		float tmp_y_scale = 0.0;
		glm::vec4 tmp_color;

		void update_push();

	public:
		float x_scale = 0.5f;
		float y_scale = 0.5f;
		glm::vec3 pos;
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		void init();
		void draw();
		inline bool is_on(float x, float y);

		void push();
		void update();
};

void Rect::update()
{
	update_push();
}

void Rect::init()
{
	program = make_program("C:\\Users\\Akash\\Documents\\Programming\\OpenGL\\Include\\rect_vertex", "C:\\Users\\Akash\\Documents\\Programming\\OpenGL\\Include\\rect_fragment");
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void Rect::update_push()
{
	if(should_push)
	{
		count++;
		if(count == 1)
		{
			tmp_x_scale = x_scale;
			tmp_y_scale = y_scale;
			tmp_color = color;
		}

		if ((glfwGetTime() - current_push_start_time) < delay_push_time)
		{
			x_scale = 0.95f * tmp_x_scale;
			y_scale = 0.95f * tmp_y_scale;
			color = 0.5f * tmp_color;
		}
		else
		{
			should_push = false;
			x_scale = tmp_x_scale;
			y_scale = tmp_y_scale;
			color = tmp_color;
			count = 0;
		}
	}
}

void Rect::push()
{
	should_push = true;
	current_push_start_time = glfwGetTime();
}

bool Rect::is_on(float x, float y)
{
	bool b = false;
	if (x > pos.x - x_scale && x < pos.x + x_scale && y > pos.y - y_scale && y < pos.y + y_scale)
		b = true;

	return b;
}

void Rect::draw()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glUseProgram(program);

	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(x_scale, y_scale, 1.0f));
	set_mat4f(program, "model", model);
	set_vec4f(program, "rect_color", color.r, color.g, color.b, color.a);

	model = glm::mat4();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void BorderRect::init()
{
	program = make_program("C:\\Users\\Akash\\Documents\\Programming\\OpenGL\\Include\\rect_vertex", "C:\\Users\\Akash\\Documents\\Programming\\OpenGL\\Include\\rect_fragment");
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void BorderRect::draw()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glUseProgram(program);

	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(x_scale, y_scale, 1.0f));
	set_mat4f(program, "model", model);
	set_vec4f(program, "rect_color", color.r, color.g, color.b, color.a);

	model = glm::mat4();

	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

#endif
