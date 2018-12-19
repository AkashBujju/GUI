#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <init.h>
#include <button.h>
#include <utils.h>

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void update();
void draw();

const int scr_width = 1024, scr_height = 768;
int mouse_x = 0, mouse_y = 0;
bool mouse_clicked = false;

Button btn, btn_2;

int main()
{
	init_glfw();
	GLFWwindow *window = make_window(scr_width, scr_height, "Text");
	init_glad();
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	btn.init("OpenGL and C++", scr_width, scr_height, "Consolas.ttf", 30);
	btn_2.init("Button is working", scr_width, scr_height, "Consolas.ttf", 20);

	btn.set_x(0.3f, scr_width);
	btn.set_y(0.3f, scr_height);
	btn.rect.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	btn.text_color = glm::vec3(0.5f, 0.5f, 0.5f);

	btn_2.set_x(-0.5f, scr_width);
	btn_2.set_y(-0.2f, scr_height);
	btn_2.rect.color = glm::vec4(0.5, 0.8f, 0.1f, 1.0f);
	btn_2.text_color = glm::vec3(0.1f, 0.1f, 0.1f);

	while (!glfwWindowShouldClose(window))
	{
		mouse_clicked = false;

		glfwPollEvents();
		processInput(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		update();
		draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void update()
{
	float norm_x = get_x_normalized(mouse_x, scr_width);
	float norm_y = get_y_normalized(mouse_y, scr_height);

	if(btn.rect.is_on(norm_x, norm_y) && mouse_clicked)
		btn.rect.push();
	else if(btn_2.rect.is_on(norm_x, norm_y) && mouse_clicked)
		btn_2.rect.push();

	btn.update();
	btn_2.update();
}

void draw()
{
	btn.draw();
	btn_2.draw();
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	mouse_x = xpos;
	mouse_y = ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_clicked = true;
	}
}
