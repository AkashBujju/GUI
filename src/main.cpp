#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <init.h>
#include <button.h>
#include <new_textbox.h>

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void update();
void draw();

const int scr_width = 1400, scr_height = 800;
double mouse_x = 0, mouse_y = 0;
bool mouse_clicked = false;
GLFWwindow *window = nullptr;

// DropDown d;
Button save_btn, load_btn;
NewTextBox ntb;

int main()
{
	init_glfw();
	window = make_window(scr_width, scr_height, "Text");
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

	ntb.init("Consolas.ttf", 20, scr_width, scr_height);
	ntb.add_text("And OpenGL");
	ntb.add_text("This is C++ Oh Oh");
	ntb.add_text("Hello, World .... !!!!!!!!");

	save_btn.init("Save", scr_width, scr_height, "Consolas.ttf", 25);
	load_btn.init("Load", scr_width, scr_height, "Consolas.ttf", 25);
	save_btn.set_x(+0.8f, scr_width);
	save_btn.set_y(+0.85f, scr_height);

	load_btn.set_x(+0.8f, scr_width);
	load_btn.set_y(+0.7f, scr_height);

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
	std::cout << "Ended successfully" << std::endl;
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

	save_btn.update();

	if(mouse_clicked && save_btn.rect.is_on(mouse_x, mouse_y))
	{
		save_btn.rect.push();	
		ntb.save("demo.txt");
	}
	else if(mouse_clicked && load_btn.rect.is_on(mouse_x, mouse_y))
	{
		load_btn.rect.push();	
		ntb.load("sample_c++.cpp");
	}
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	int right_state, left_state, up_state, down_state;
	int erase_state, return_state, esc_state;
	int i_state;

	right_state = glfwGetKey(window, GLFW_KEY_L);
	left_state = glfwGetKey(window, GLFW_KEY_H);
	up_state = glfwGetKey(window, GLFW_KEY_K);
	down_state = glfwGetKey(window, GLFW_KEY_J);
	erase_state = glfwGetKey(window, GLFW_KEY_BACKSPACE);
	return_state = glfwGetKey(window, GLFW_KEY_ENTER);
	esc_state = glfwGetKey(window, GLFW_KEY_ESCAPE);

	if(ntb.mode == Mode::ESC)
	{
		if(right_state == GLFW_PRESS)
			ntb.go_next_char();
		else if(left_state == GLFW_PRESS)
			ntb.go_prev_char();
		else if(up_state == GLFW_PRESS)
			ntb.go_prev_line();
		else if(down_state == GLFW_PRESS)
			ntb.go_next_line();
	}

	if(esc_state == GLFW_PRESS)
		ntb.goto_esc_mode();
	else if(return_state == GLFW_PRESS)
		ntb.add_new_line();
	else if(erase_state == GLFW_PRESS)
		ntb.erase();
}

void draw()
{
	// d.draw();
	// tb.draw();
	ntb.draw();
	save_btn.draw();
	load_btn.draw();
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	mouse_x = get_x_normalized(xpos, scr_width);
	mouse_y = get_y_normalized(ypos, scr_height);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	char ch = (char)codepoint;
	ntb.insert(ch);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_clicked = true;
	}
}
