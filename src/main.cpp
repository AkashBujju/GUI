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
#include <image.h>
#include <drop_down.h>
#include <utils.h>
#include <rect.h>
#include <text_box.h>

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void update();
void draw();

const int scr_width = 1024, scr_height = 800;
double mouse_x = 0, mouse_y = 0;
bool mouse_clicked = false;

// DropDown d;
TextBox tb;

int main()
{
	init_glfw();
	GLFWwindow *window = make_window(scr_width, scr_height, "Text");
	init_glad();

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);

	/*
	d.init(0.0f, 0.0f);
	d.add_item("Item_1", scr_width, scr_height, 20);
	d.add_item("Item_2....", scr_width, scr_height, 20);
	d.add_item("Item_3.", scr_width, scr_height, 20);
	d.set_x(-0.6f, scr_width);
	d.set_y(0.0f, scr_height);
	d.set_to(2, scr_width, scr_height);
	*/

	tb.init(scr_width, scr_height);
	tb.add_text("#include <iostream>");
	tb.add_text("#include <vector>");
	tb.add_text("int main() {");
	tb.add_text("  std::cout << \"HelloWorld\" << std::endl;");
	tb.add_text("  return 0;");
	tb.add_text("}");


	tb.cursor.x_scale = tb.get_norm_char_w() / 2.0f;
	tb.cursor.y_scale = tb.get_norm_char_h() / 2.0f;

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
	/*
	if(mouse_clicked && d.image.is_on(mouse_x, mouse_y))
	{
		d.toggle_box();
	}
	if(mouse_clicked)
	{
		d.switch_to(mouse_x, mouse_y, scr_width, scr_height);
	}
	*/
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);
}

void draw()
{
	// d.draw();
	tb.draw();
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	mouse_x = get_x_normalized(xpos, scr_width);
	mouse_y = get_y_normalized(ypos, scr_height);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if((char)codepoint == 'w')
		tb.goto_prev_line();
	else if((char)codepoint == 's')
		tb.goto_next_line();
	else if((char)codepoint == 'a')
		tb.goto_prev_char();
	else if((char)codepoint == 'd')
		tb.goto_next_char();
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_clicked = true;
	}
}
