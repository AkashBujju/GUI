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
#include <utils.h>

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void update();
void draw();

const int scr_width = 1024, scr_height = 768;
int mouse_x = 0, mouse_y = 0;
bool mouse_clicked = false;

Button btn;
Circle circle;

int main()
{
	init_glfw();
	GLFWwindow *window = make_window(scr_width, scr_height, "Text");
	init_glad();
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glfwSetKeyCallback(window, key_callback);

	btn.init("1", scr_width, scr_height, "Consolas.ttf", 30);
	btn.set_x(0.3f, scr_width);
	btn.set_y(0.3f, scr_height);
	btn.rect.color = glm::vec4(0.2f, 0.5f, 0.2f, 1.0f);
	btn.text_color = glm::vec3(0.5f, 0.8f, 0.3f);

	circle.init(0.0f, 0.0f, 0.1f, 1.0f);
	circle.color = glm::vec3(0.2f, 0.5f, 0.6f);

	while (!glfwWindowShouldClose(window))
	{
		mouse_clicked = false;
		glfwPollEvents();

		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if(state == GLFW_PRESS)
			std::cout << "Pressed" << std::endl;

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

	btn.update();
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);
	else if(key == GLFW_KEY_A && action == GLFW_REPEAT)
	{
		std::cout << "A" << std::endl;
	}
}

void draw()
{
	btn.draw();
	circle.draw();
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
