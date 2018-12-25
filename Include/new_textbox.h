/* A redo of text_box.h */
#ifndef NEW_TEXTBOX_H
#define NEW_TEXTBOX_H

#include <iostream>
#include <vector>
#include <rect.h>
#include <font.h>
#include <utils.h>
#include <fstream>
#include <deque>

struct Text
{
	Font font;
	std::string text;
	glm::vec2 pos;
};

struct NewTextBox
{
	Rect box;
	Rect cursor;
	std::vector<Text*> texts;

	glm::vec2 up_left_org;
	glm::vec2 box_dims_org;

	int current_line_index = 0;
	int current_char_index = 0;

	unsigned int font_size;
	unsigned int scr_width = 0;
	unsigned int scr_height = 0;
	unsigned int text_gap_x_org = 10;
	unsigned int text_gap_y_org = 10;
	float text_gap_x_norm = 0;
	float text_gap_y_norm = 0;
	float cache_font_width_norm = 0;
	float cache_font_height_norm = 0;

	std::string font_name;

	void init(std::string font_name, unsigned int font_size, unsigned int w, unsigned int h);
	void draw();
	void add_text(std::string text);
	void go_next_char();
	void go_prev_char();
	void go_next_line();
	void go_prev_line();
};

void NewTextBox::init(std::string font_name, unsigned int font_size, unsigned int w, unsigned int h)
{
	scr_width = w;
	scr_height = h;

	this->font_name = font_name;
	this->font_size = font_size;

	text_gap_x_norm = text_gap_x_org / (float)scr_width;
	text_gap_y_norm = text_gap_y_org / (float)scr_height;

	box.init();
	box.x_scale = 0.5f;
	box.y_scale = 0.5f;
	box.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	int tmp_x = get_org_x(box.pos.x, scr_width);
	int tmp_y = get_org_y(box.pos.y, scr_height);
	int box_w = (box.x_scale / 2.0f) * scr_width;
	int box_h = (box.y_scale / 2.0f) * scr_height;

	if(box.pos.x == 0)
		tmp_x = scr_width / 2.0f;

	if(tmp_y < 0)
		tmp_y = abs(tmp_y);
	else
		tmp_y = tmp_y + (scr_height / 2.0f);

	up_left_org.x = tmp_x;
	up_left_org.y = tmp_y;
	box_dims_org.x = box_w;
	box_dims_org.y = box_h;

	Text cache_text;
	cache_text.font.init_lib();
	cache_text.font.init_program(scr_width, scr_height);
	cache_text.font.make_buffer();
	cache_text.font.init_font("A", font_name, font_size);
	cache_font_width_norm = cache_text.font.get_width("A") / (float)scr_width;
	cache_font_height_norm = cache_text.font.get_height("A") / (float)scr_height;

	cursor.init();
	cursor.pos.x = box.pos.x - box.x_scale + 2.0f * text_gap_x_norm + cache_font_width_norm;
	cursor.pos.y = box.pos.y + box.y_scale - text_gap_y_norm - cache_font_height_norm;
	cursor.x_scale = cache_font_width_norm;
	cursor.y_scale = cache_font_height_norm * 2.0f;
	cursor.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void NewTextBox::go_next_char()
{
	current_char_index++;
	cursor.pos.x += cache_font_width_norm * 2.0f;	
}

void NewTextBox::go_prev_char()
{
	current_char_index--;
	cursor.pos.x -= cache_font_width_norm * 2.0f;	
}

void NewTextBox::go_next_line()
{
	current_line_index++;
	cursor.pos.y -= (4.0f * cache_font_height_norm + 1.0f * text_gap_y_norm);
}

void NewTextBox::go_prev_line()
{
	current_line_index--;
	cursor.pos.y += (4.0f * cache_font_height_norm + 1.0f * text_gap_y_norm);
}

void NewTextBox::add_text(std::string text)
{
	Text *t = new Text;
	t->text = text;

	t->font.init_lib();
	t->font.init_program(scr_width, scr_height);
	t->font.make_buffer();
	t->font.init_font(text, font_name, font_size);
	texts.push_back(t);

	if(texts.size() == 1)
	{
		t->pos.x = up_left_org.x - box_dims_org.x + text_gap_x_org;
		t->pos.y = up_left_org.y + box_dims_org.y;
		t->pos.y -= font_size;
	}
	else
	{
		Text *prev = texts[texts.size() - 2];
		t->pos.x = up_left_org.x - box_dims_org.x + text_gap_x_org;
		t->pos.y = prev->pos.y - font_size - text_gap_y_org;
	}
}

void NewTextBox::draw()
{
	box.draw();
	cursor.draw();

	for(unsigned int i = 0; i < texts.size(); i++)
	{
		texts[i]->font.render_text(texts[i]->text, texts[i]->pos.x, texts[i]->pos.y, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

#endif
/* A redo of text_box.h */
