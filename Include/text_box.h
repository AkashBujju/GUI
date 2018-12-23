#ifndef MY_TEXT_BOX_H
#define MY_TEXT_BOX_H

#include <iostream>
#include <vector>
#include <rect.h>
#include <font.h>
#include <utils.h>

struct Text
{
	std::string text;
	glm::vec2 pos;
	Font font;

	glm::vec2 norm_pos;
};

struct TextBox
{
	Rect box;
	Rect cursor;
	glm::vec2 up_left;
	glm::vec2 current_line;
	std::vector<Text*> texts;

	unsigned int scr_width = 0;
	unsigned int scr_height = 0;

	unsigned int current_line_index = 0;
	unsigned int current_char_index = 0;

	void draw();
	void init(unsigned int w, unsigned int h);
	void set_text_pos_x(Text *text, float x, int scr_width);
	void set_text_pos_y(Text *text, float y, int scr_height);

	void add_text(std::string text);

	float get_norm_char_w();
	float get_norm_char_h();

	void goto_next_char();
	void goto_next_line();
	void goto_prev_char();
	void goto_prev_line();
};

void TextBox::goto_prev_char()
{
	if(current_char_index < 0)
		current_char_index = 0;
	else
		current_char_index--;

	cursor.pos.x -= get_norm_char_w();
}

void TextBox::goto_next_char()
{
	if(current_char_index > texts[current_line_index]->text.size() - 1)
	{
		current_char_index = texts[current_line_index]->text.size() - 1;
	}
	else
		current_char_index++;

	cursor.pos.x += get_norm_char_w();
}

void TextBox::goto_prev_line()
{
	if(current_line_index == 0)
		return;

	current_line_index--;
	cursor.pos.y = texts[current_line_index]->norm_pos.y + cursor.y_scale / 2.0f;
}

void TextBox::goto_next_line()
{
	if(current_line_index == texts.size() - 1)
		return;

	current_line_index++;
	cursor.pos.y = texts[current_line_index]->norm_pos.y + cursor.y_scale / 2.0f;
}

float TextBox::get_norm_char_w()
{
	float full_width = texts[0]->font.get_width(texts[0]->text) / (float)(scr_width);

	float per_width = full_width / (float)texts[0]->text.size();

	return per_width * 2;
}

float TextBox::get_norm_char_h()
{
	return texts[0]->font.get_height(texts[0]->text) * 4.0f / (float)scr_height;
}

void TextBox::add_text(std::string text)
{
	Text *t = new Text;
	t->text = text;

	t->font.init_lib();
	t->font.init_program(scr_width, scr_height);
	t->font.make_buffer();
	t->font.init_font(text, "Consolas.ttf", 15);

	set_text_pos_x(t, current_line.x, scr_width);
	set_text_pos_y(t, current_line.y, scr_height);

	texts.push_back(t);

	current_line.y -= (t->font.get_height(text) * 4.0f) / (float)scr_height;
}

void TextBox::init(unsigned int w, unsigned int h)
{
	scr_width = w;
	scr_height = h;

	box.init();
	box.pos.x = 0;
	box.pos.y = 0;
	box.x_scale = 0.5f;
	box.y_scale = 0.5f;
	box.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	up_left = glm::vec2(box.pos.x - box.x_scale + 0.01f, box.pos.y + box.y_scale - 0.01f);
	current_line = up_left;

	cursor.init();
	cursor.x_scale = 0.008f;
	cursor.y_scale = 0.015f;
	cursor.pos.x = current_line.x + cursor.x_scale;
	cursor.pos.y = current_line.y - cursor.y_scale;
	cursor.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void TextBox::draw()
{
	box.draw();
	cursor.draw();

	for(unsigned int i = 0; i < texts.size(); i++)
	{
		texts[i]->font.render_text(texts[i]->text, texts[i]->pos.x, texts[i]->pos.y, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void TextBox::set_text_pos_x(Text *text, float x, int scr_width)
{
	float norm_w = text->font.get_width(text->text) / (float)scr_width;
	// x -= norm_w;

	text->norm_pos.x = x;

	float final_norm_x = 0;
	if(x > 0)
	{
		float t = x + 1.0f;
		final_norm_x = (float)t / 2.0f;
	}
	else
	{
		float t = abs(1 + x);
		final_norm_x = (float)t / 2.0f;
	}

	text->pos.x = final_norm_x * scr_width;
}

void TextBox::set_text_pos_y(Text *text, float y, int scr_height)
{
	float norm_h = text->font.get_height(text->text) / (float)scr_height;
	y -= 2 * norm_h;
	text->norm_pos.y = y;

	float final_norm_y = 0;
	if(y > 0)
	{
		float t = 1.0f - y;
		final_norm_y = (float)t / 2.0f;
	}
	else
	{
		float t = abs(1 - y);
		final_norm_y = (float)t / 2.0f;
	}

	text->pos.y = scr_height - (final_norm_y * scr_height);
}

#endif
