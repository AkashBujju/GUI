#ifndef MY_TEXTBOX_H
#define MY_TEXTBOX_H

#include <string>
#include <button.h>
#include <font.h>
#include <rect.h>
#include <utils.h>
#include <glm/glm.hpp>

struct TextBox
{
	Rect outer_panel;
	Rect inner_panel;
	Rect cursor;
	Font text;
	Button btn;

	bool is_active = false;
	std::string str;
	glm::vec2 text_pos;
	float font_width_norm = 0;
	int char_index = -1;

	void init(unsigned int scr_width, unsigned int scr_height);
	void draw();
	void go_next_char();
	void go_prev_char();
	void insert(char ch);
	void check_and_toggle_is_active(float x_norm, float y_norm);
	std::string check_and_get_str(float x_norm, float y_norm);
};

void TextBox::init(unsigned int scr_width, unsigned int scr_height)
{
	outer_panel.init();
	outer_panel.x_scale = 0.2f;
	outer_panel.y_scale = 0.05f;
	outer_panel.color = glm::vec4(0.5, 0.0f, 0.0f, 1.0f);

	inner_panel.init();
	inner_panel.x_scale = outer_panel.x_scale / 1.1f;
   inner_panel.y_scale = outer_panel.y_scale / 1.1f;
	inner_panel.color = glm::vec4(0.5, 0.5f, 0.5f, 1.0f);

	cursor.init();
	cursor.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	cursor.x_scale = 0.0015f;
	cursor.y_scale = 0.03f;
	cursor.pos.x = inner_panel.pos.x - inner_panel.x_scale + 4 * cursor.x_scale;

	btn.init("Ok", scr_width, scr_height, "Consolas.ttf", 18);
	btn.set_x(outer_panel.pos.x + outer_panel.x_scale + btn.rect.x_scale, scr_width);
	btn.rect.y_scale = outer_panel.y_scale;
	btn.rect.color = glm::vec4(0.1f, 0.1f, 0.3f, 1.0f);

	str = "Enter here";
	text.init_lib();
	text.init_program(scr_width, scr_height);
	text.make_buffer();
	text.init_font(str, "Consolas.ttf", 18);

	font_width_norm = text.get_width("A") / (float)scr_width;

	text_pos.x = get_org_x(cursor.pos.x, scr_width);
	float ty = get_org_y(cursor.pos.y, scr_height);
	if(ty < 0)
		ty = abs(ty);
	else
		ty = ty + (scr_height / 2.0f);
	text_pos.y = ty - text.get_height(str) / 2.0f;
}   

void TextBox::go_next_char()
{
	if(char_index == str.size())
		return;

	cursor.pos.x += font_width_norm * 2.0f;
}

void TextBox::go_prev_char()
{
	if(char_index <= 0)
		return;

	cursor.pos.x -= font_width_norm * 2.0f;
	char_index--;
}

void TextBox::insert(char ch)
{
	if(!is_active)
		return;

	std::string tmp;
	tmp.push_back(ch);

	str.insert(char_index, tmp);

	char_index++;
	cursor.pos.x += font_width_norm * 2.0f;
}

void TextBox::check_and_toggle_is_active(float x_norm, float y_norm)
{
	if(outer_panel.is_on(x_norm, y_norm))
	{
		is_active = true;

		str = "";
		cursor.pos.x = inner_panel.pos.x - inner_panel.x_scale + 4 * cursor.x_scale;
		char_index = 0;
	}
}

std::string TextBox::check_and_get_str(float x_norm, float y_norm)
{
	std::string s = "";
	if (btn.rect.is_on(x_norm, y_norm))
	{
		btn.rect.push();
		is_active = false;

		s = str;
		str = "Enter here";
		cursor.pos.x = inner_panel.pos.x - inner_panel.x_scale + 4 * cursor.x_scale;
		char_index = 0;
	}

	return s;
}

void TextBox::draw()
{
	outer_panel.draw();
	inner_panel.draw();
	cursor.draw();
	btn.draw();

	text.render_text(str, text_pos.x, text_pos.y, 1.0f, glm::vec3(0.05f, 0.05f, 0.05f));
}

#endif
