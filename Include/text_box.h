#ifndef MY_TEXT_BOX_H
#define MY_TEXT_BOX_H

#include <iostream>
#include <vector>
#include <rect.h>
#include <font.h>
#include <utils.h>
#include <fstream>

struct Text
{
	Font font;
	std::string text;
	glm::vec2 pos;
	glm::vec2 norm_pos;
};

struct TextBox
{
	Rect box;
	Rect cursor;
	glm::vec2 up_left;
	glm::vec2 current_line;
	std::vector<Text*> texts;

	Font cache_font;
	unsigned int cache_font_width = 0;
	unsigned int cache_font_height = 0;

	std::string font_name;
	unsigned int font_size;

	void draw();
	void init(std::string font_name, unsigned int font_size, unsigned int w, unsigned int h);
	void add_text(std::string text);

	float get_norm_char_w();
	float get_norm_char_h();
	void goto_next_char();
	void goto_next_line();
	void goto_prev_char();
	void goto_prev_line();
	void insert(char ch);
	void erase();
	void add_new_line();

	void save(std::string f_name);

	~TextBox();
private:
	unsigned int scr_width = 0;
	unsigned int scr_height = 0;
	unsigned int current_line_index = 0;
	unsigned int current_char_index = 0;
	void set_text_pos_x(Text *text, float x, int scr_width);
	void set_text_pos_y(Text *text, float y, int scr_height);
	void check_and_set_to_max_or_min();
};

void TextBox::save(std::string f_name)
{
	std::ofstream wr(f_name);
	if(!wr)
	{
		std::cout << "Cannot open file: " << f_name << " for writing" << std::endl;
		return;
	}

	for(unsigned int i = 0; i < texts.size(); i++)
	{
		wr << texts[i]->text;
		wr << "\n";
	}

	wr.close();
}

TextBox::~TextBox()
{
	for(unsigned int i = 0; i < texts.size(); i++)
	{
		delete texts[i];
	}
}

void TextBox::add_new_line()
{
	// Insert a blank line below current_line_index
	Text *t = new Text;
	t->text = "";
	t->font.init_lib();
	t->font.init_program(scr_width, scr_height);
	t->font.make_buffer();
	t->font.init_font(t->text, font_name, font_size);

	std::vector<Text*>::iterator it = texts.begin();
	texts.insert(it + current_line_index + 1, t);

	unsigned int tmp_li = current_line_index + 1;

	current_line.y = texts[current_line_index]->norm_pos.y;
	current_line.y -= (cache_font_height * 2.0f) / (float)scr_height;
	set_text_pos_x(t, current_line.x, scr_width);
	set_text_pos_y(t, current_line.y, scr_height);

	current_line_index += 1;
	for(unsigned int i = current_line_index; i < texts.size(); i++)
	{
		current_line.y = texts[i]->norm_pos.y;
		current_line.y -= (cache_font_height * 2.0f) / (float)scr_height;
		set_text_pos_y(texts[i], current_line.y, scr_height);
	}

	current_line_index = tmp_li;
	current_line.y = texts[current_line_index]->norm_pos.y;
	cursor.pos.y = texts[current_line_index]->norm_pos.y + cursor.y_scale / 2.0f;
	cursor.pos.x = up_left.x + cursor.x_scale;

	current_char_index = 0;
}

void TextBox::erase()
{
	texts[current_line_index]->text.erase(current_char_index, 1);
}

void TextBox::insert(char ch)
{
	std::string tmp;
	tmp.push_back(ch);
	texts[current_line_index]->text.insert(current_char_index, tmp);

	current_char_index++;
	cursor.pos.x += get_norm_char_w();
}

void TextBox::check_and_set_to_max_or_min()
{
	if(current_char_index > texts[current_line_index]->text.size() - 1)
	{
		cursor.pos.x = current_line.x + cursor.x_scale;
		current_char_index = 0;
		for(unsigned int i = 0; i < texts[current_line_index]->text.size() - 1; i++)
		{
			cursor.pos.x += get_norm_char_w();
			current_char_index++;
		}
	}
}

void TextBox::goto_prev_char()
{
	if(current_char_index == 0)
		return;
	else
		current_char_index--;

	cursor.pos.x -= get_norm_char_w();
}

void TextBox::goto_next_char()
{
	if(current_char_index == texts[current_line_index]->text.size())
		return;
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

	check_and_set_to_max_or_min();
}

void TextBox::goto_next_line()
{
	if(current_line_index == texts.size() - 1)
		return;

	current_line_index++;
	cursor.pos.y = texts[current_line_index]->norm_pos.y + cursor.y_scale / 2.0f;

	check_and_set_to_max_or_min();
}

float TextBox::get_norm_char_w()
{
	float per_width = cache_font_width / (float)(scr_width);
	return per_width * 2;
}

float TextBox::get_norm_char_h()
{
	return cache_font_height * 4.0f / (float)scr_height;
}

void TextBox::add_text(std::string text)
{
	Text *t = new Text;
	t->text = text;

	t->font.init_lib();
	t->font.init_program(scr_width, scr_height);
	t->font.make_buffer();
	t->font.init_font(text, font_name, font_size);

	set_text_pos_x(t, current_line.x, scr_width);
	set_text_pos_y(t, current_line.y, scr_height);

	texts.push_back(t);

	current_line.y = texts[texts.size() - 1]->norm_pos.y;
	current_line.y -= (cache_font_height * 2.0f) / (float)scr_height;
}

void TextBox::init(std::string font_name, unsigned int font_size, unsigned int w, unsigned int h)
{
	scr_width = w;
	scr_height = h;

	this->font_name = font_name;
	this->font_size = font_size;

	box.init();
	box.pos.x = 0;
	box.pos.y = 0;
	box.x_scale = 0.5f;
	box.y_scale = 0.5f;
	box.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	cache_font.init_lib();
	cache_font.init_program(scr_width, scr_height);
	cache_font.make_buffer();
	cache_font.init_font("A", font_name, font_size);
	cache_font_width = cache_font.get_width("A");
	cache_font_height = cache_font.get_height("A");

	up_left = glm::vec2(box.pos.x - box.x_scale + 0.01f, box.pos.y + box.y_scale - 0.01f);
	current_line = up_left;

	cursor.init();
	cursor.x_scale = get_norm_char_w() / 2.0f;
	cursor.y_scale = get_norm_char_h() / 2.0f;
	cursor.pos.x = current_line.x + cursor.x_scale;
	cursor.pos.y = current_line.y - cursor.y_scale;
	cursor.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	current_line.y -= ((cache_font_height * 2.5f) / (float)scr_height);

	add_text("");
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
