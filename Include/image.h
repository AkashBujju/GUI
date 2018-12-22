#ifndef MY_IMAGE_H
#define MY_IMAGE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <texture.h>

static float image_vertices[] = {
	+1.0f, +1.0f, 0.0f, 1.0f, 1.0f,
	+1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, +1.0f, 0.0f, 0.0f, 1.0f
};

static unsigned int image_indices[] = {
	0, 1, 3,
	1, 2, 3
};

struct Image
{
	private:
		unsigned int vao, vbo, ebo;
		unsigned int program = -1;
		unsigned int texture = -1;
		glm::mat4 model;
		const float delay_push_time = 0.2f;
		float current_push_start_time = 0.0f;
		bool should_push = false;
		int count = 0;

		float tmp_x_scale = 0.0;
		float tmp_y_scale = 0.0;

		void update_push();

	public:
		float x_scale = 0.5f;
		float y_scale = 0.5f;
		glm::vec3 pos;

		void init(std::string fname);
		void draw();
		void push();
		void update();
		inline bool is_on(float x, float y);
};

bool Image::is_on(float x, float y)
{
	bool b = false;
	if (x > pos.x - x_scale && x < pos.x + x_scale && y > pos.y - y_scale && y < pos.y + y_scale)
		b = true;

	return b;
}

void Image::init(std::string fname)
{
	program = make_program("C:\\Users\\Akash\\Documents\\GitHub\\GUI\\Include\\image_vertex", "C:\\Users\\Akash\\Documents\\GitHub\\GUI\\Include\\image_fragment");

	texture = make_texture(fname.c_str());

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(image_vertices), image_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(image_indices), image_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Image::draw()
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(program);

	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(x_scale, y_scale, 1.0f));
	set_mat4f(program, "model", model);

	model = glm::mat4();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

#endif
