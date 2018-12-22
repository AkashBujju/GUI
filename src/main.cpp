#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <init.h>
#include <button.h>
#include <circle.h>
#include <drop_down.h>
#include <utils.h>

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void update();
void draw();

const int scr_width = 600, scr_height = 600;
double mouse_x = 0, mouse_y = 0;
bool mouse_clicked = false;

DropDown d;

int main()
{
	init_glfw();
	GLFWwindow *window = make_window(scr_width, scr_height, "Text");
	init_glad();
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glfwSetKeyCallback(window, key_callback);

	d.init(0.0f, 0.0f);
	d.add_item("Item_1", scr_width, scr_height, 20);
	d.add_item("Item_222222", scr_width, scr_height, 20);
	d.add_item("Item_3", scr_width, scr_height, 20);
	d.set_x(-0.5f, scr_width);
	d.set_y(0.0f, scr_height);
	d.set_to(2, scr_width, scr_height);

	while (!glfwWindowShouldClose(window))
	{
		mouse_clicked = false;
		glfwPollEvents();

		/*
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if(state == GLFW_PRESS)
			std::cout << "Pressed" << std::endl;
		*/

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
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);
}

void draw()
{
	d.draw();
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	mouse_x = get_x_normalized(xpos, scr_width);
	mouse_y = get_y_normalized(ypos, scr_height);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_clicked = true;
	}
}
