#ifndef MY_DROPDOWN_H
#define MY_DROPDOWN_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <button.h>
#include <image.h>
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
		bool is_pulled_up = false;

		void pull_up();
		void pull_down();
	public:
		Image image;

		void init(float x, float y);
		void add_item(std::string text, unsigned int scr_width, unsigned int scr_height, unsigned int _fsize);
		void draw();
		void set_x(float x, unsigned int scr_width);
		void set_y(float y, unsigned int scr_height);
		void set_to(unsigned int index, unsigned int scr_width, unsigned int scr_height);

		void toggle_box();
		~DropDown();
};

void DropDown::toggle_box()
{
	if(is_pulled_up)
		pull_down();
	else
		pull_up();
}

void DropDown::pull_up()
{
	is_pulled_up = true;
	if(buttons.size() == 0)
		return;

	panel.pos.y = buttons[0]->rect.pos.y;
	panel.y_scale = buttons[0]->rect.y_scale * 1.25f;

	panel_border.pos.y = panel.pos.y;
	panel_border.y_scale = panel.y_scale * 1.01f;
}

void DropDown::pull_down()
{
	is_pulled_up = false;
	if(buttons.size() == 0)
		return;

	float middle_x = b_rects[0]->pos.x - b_rects[0]->x_scale + panel.x_scale;
	float middle_y = b_rects[0]->pos.y + b_rects[0]->y_scale;
	middle_y -= (b_rects[0]->y_scale * b_rects.size() * 1.15f);
	panel.pos.y = middle_y;
	panel.y_scale = (b_rects[0]->y_scale * b_rects.size() * 1.25f);
	panel_border.pos.y = panel.pos.y;
	panel_border.y_scale = panel.y_scale * 1.01f;
}

void DropDown::set_to(unsigned int index, unsigned int scr_width, unsigned int scr_height)
{
	if(buttons.size() == 0 || index > buttons.size() - 1)
		return;

	// Swap pos of Index 0 with the above index
	float tmp_button_y = buttons[index]->text_pos_y;
	buttons[index]->text_pos_y = buttons[0]->text_pos_y;
	buttons[0]->text_pos_y = tmp_button_y;

	float tmp_rect_y = buttons[index]->rect.pos.y;
	buttons[index]->rect.pos.y = buttons[0]->rect.pos.y; 
	buttons[0]->rect.pos.y = tmp_rect_y;

	float tmp_brects_y = b_rects[index]->pos.y;
	b_rects[index]->pos.y = b_rects[0]->pos.y;
	b_rects[0]->pos.y = tmp_brects_y;

	// Also exchanging their indices
	Button* _btn_index = buttons[index];
	buttons[index] = buttons[0];
	buttons[0] = _btn_index;

	BorderRect* tmp_rect = b_rects[index];
	b_rects[index] = b_rects[0];
	b_rects[0] = tmp_rect;
}

void DropDown::set_x(float x, unsigned int scr_width)
{
	panel.pos.x = x;
	panel_border.pos.x = panel.pos.x;
	panel_border.x_scale = panel.x_scale * 1.01f;
	up_left.x = panel.pos.x - panel.x_scale + 0.04f;
	current_add_pos.x = up_left.x;
	image.pos.x = panel.pos.x + panel.x_scale + image.x_scale;

	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->set_x(current_add_pos.x + buttons[i]->rect.x_scale, scr_width);
	}
}

void DropDown::set_y(float y, unsigned int scr_height)
{
	panel.pos.y = y;
	panel_border.pos.y = panel.pos.y;
	panel_border.y_scale = panel.y_scale * 1.01f;
	up_left.y = panel.pos.y + panel.y_scale - 0.04f;
	current_add_pos.y = up_left.y;
	image.pos.y = panel.pos.y + 2 * image.y_scale;

	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->set_y(current_add_pos.y - buttons[i]->rect.y_scale, scr_height);
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
	panel.x_scale = 0.3f;
	panel.pos.x = x;
	panel.pos.y = y;

	image.init("C:\\Users\\Akash\\Documents\\GitHub\\GUI\\src\\down_arrow_2.png");
	image.x_scale = 0.1f;
	image.y_scale = 0.1f;

	panel_border.init();
	panel_border.color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
	panel_border.pos.x = panel.pos.x - 0.03f;
	panel_border.pos.y = panel.pos.y + 0.03f;

	up_left = glm::vec3(panel.pos.x - panel.x_scale + 0.04f, panel.pos.y + panel.y_scale - 0.04f, 0.0f);

	current_add_pos = up_left;
}

void DropDown::add_item(std::string text, unsigned int scr_width, unsigned int scr_height, unsigned int _fsize)
{
	Button *tmp_btn = new Button;
	tmp_btn->init(text, scr_width, scr_height, "Consolas.ttf", _fsize);
	tmp_btn->set_x(current_add_pos.x + tmp_btn->rect.x_scale, scr_width);
	tmp_btn->set_y(current_add_pos.y - tmp_btn->rect.y_scale, scr_height);
	tmp_btn->rect.color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);

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

	image.pos.y = panel.pos.y + 2 * image.y_scale * 1.05f;
	image.pos.x = panel.pos.x + panel.x_scale + image.x_scale;
}

void DropDown::draw()
{
	image.draw();
	panel.draw();
	panel_border.draw();
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		if(is_pulled_up)
		{
			buttons[0]->draw();
			b_rects[0]->draw();
			return;
		}
		else
		{
			buttons[i]->draw();
			b_rects[i]->draw();
		}
	}
}

#endif
