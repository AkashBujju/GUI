#ifndef MY_DROPDOWN_H
#define MY_DROPDOWN_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <button.h>
#include <rect.h>

struct DropDown
{
	private:
		glm::vec3 up_left;
		glm::vec3 current_add_pos;

		Rect panel;
		BorderRect panel_border;
		std::vector<Button*> buttons;
		std::vector<BorderRect*> b_rects;

	public:
		void init(float x, float y);
		void add_item(std::string text);
		void draw();

		void set_x(float x);
		void set_y(float y);

		~DropDown();
};

void DropDown::set_x(float x)
{
	panel.pos.x = x;
	panel_border.pos.x = panel.pos.x;
	panel_border.x_scale = panel.x_scale * 1.01f;
	up_left.x = panel.pos.x - panel.x_scale + 0.04f;
	current_add_pos.x = up_left.x;

	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->set_x(current_add_pos.x + buttons[i]->rect.x_scale, 600);
	}
}

void DropDown::set_y(float y)
{
	panel.pos.y = y;
	panel_border.pos.y = panel.pos.y;
	panel_border.y_scale = panel.y_scale * 1.01f;
	up_left.y = panel.pos.y + panel.y_scale - 0.04f;
	current_add_pos.y = up_left.y;

	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->set_y(current_add_pos.y - buttons[i]->rect.y_scale, 600);
		b_rects[i]->pos = buttons[i]->rect.pos;

		current_add_pos.y -= (2.4f * b_rects[i]->y_scale);
	}
}

DropDown::~DropDown()
{
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		delete buttons[i];
		delete b_rects[i];
	}
}

void DropDown::init(float x, float y)
{
	panel.init();
	panel.color = glm::vec4(0.2f, 0.3f, 0.2f, 1.0f);
	panel.pos.x = x;
	panel.pos.y = y;

	panel_border.init();
	panel_border.color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
	panel_border.pos.x = panel.pos.x - 0.03f;
	panel_border.pos.y = panel.pos.x + 0.03f;

	up_left = glm::vec3(panel.pos.x - panel.x_scale + 0.04f, panel.pos.y + panel.y_scale - 0.04f, 0.0f);

	current_add_pos = up_left;
}

void DropDown::add_item(std::string text)
{
	Button *tmp_btn = new Button;
	tmp_btn->init(text, 600, 600, "Consolas.ttf", 20);
	tmp_btn->set_x(current_add_pos.x + tmp_btn->rect.x_scale, 600);
	tmp_btn->set_y(current_add_pos.y - tmp_btn->rect.y_scale, 600);

	BorderRect *tmp_brect = new BorderRect;
	tmp_brect->init();
	tmp_brect->pos = tmp_btn->rect.pos;
	tmp_brect->x_scale = tmp_btn->rect.x_scale + 0.005f;
	tmp_brect->y_scale = tmp_btn->rect.y_scale + 0.005f;
	tmp_brect->color = glm::vec4(0.6f, 0.6f, 0.0f, 1.0f);

	buttons.push_back(tmp_btn);
	b_rects.push_back(tmp_brect);

	current_add_pos.y -= (2.4f * tmp_brect->y_scale);

	float middle_x = b_rects[0]->pos.x - b_rects[0]->x_scale + panel.x_scale;
	float middle_y = b_rects[0]->pos.y + b_rects[0]->y_scale;
	middle_y -= (b_rects[0]->y_scale * b_rects.size() * 1.15f);
	panel.pos.y = middle_y;
	panel.y_scale = (b_rects[0]->y_scale * b_rects.size() * 1.25f);

	panel_border.pos.x = panel.pos.x;
	panel_border.pos.y = panel.pos.y;
	panel_border.x_scale = panel.x_scale * 1.01f;
	panel_border.y_scale = panel.y_scale * 1.01f;
}

void DropDown::draw()
{
	panel.draw();
	panel_border.draw();
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->draw();
		b_rects[i]->draw();
	}
}

#endif
