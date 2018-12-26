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
	std::deque<Text*> page;

	glm::vec2 up_left_org;
	glm::vec2 box_dims_org;

	int current_line_index = 0;
	int current_char_index = 0;
	int current_page_line_index = 0;

	unsigned int font_size;
	unsigned int scr_width = 0;
	unsigned int scr_height = 0;
	unsigned int text_gap_x_org = 10;
	unsigned int text_gap_y_org = 10;
	unsigned int max_lines_per_page = 2;
	float text_gap_x_norm = 0;
	float text_gap_y_norm = 0;
	float cache_font_width_norm = 0;
	float cache_font_height_norm = 0;
	float next_y_norm = 0;

	std::string font_name;

	void init(std::string font_name, unsigned int font_size, unsigned int w, unsigned int h);
	void draw();
	void add_text(std::string text);
	void go_next_char();
	void go_prev_char();
	void go_next_line();
	void go_prev_line();
	void set_char_max();
	void insert(char ch);
	void save(std::string f_name);
	void load(std::string f_name);
	void scroll_down();
	void scroll_up();
	void add_new_line();
	void erase();
};

void NewTextBox::erase()
{
	if(current_char_index == 0)
		return;

	texts[current_line_index]->text.erase(current_char_index - 1, 1);
	go_prev_char();
}

void NewTextBox::add_new_line()
{
	// Insert a blank line below current_line_index
	Text *t = new Text;
	t->text = "";
	t->font.init_lib();
	t->font.init_program(scr_width, scr_height);
	t->font.make_buffer();
	t->font.init_font(t->text, font_name, font_size);
	t->pos.x = up_left_org.x - box_dims_org.x + text_gap_x_org;

	std::vector<Text*>::iterator it = texts.begin();
	texts.insert(it + current_line_index + 1, t);

	std::deque<Text*>::iterator it_2 = page.begin();
	if(texts.size() - 1 >= max_lines_per_page)
		page.pop_back();
	page.insert(it_2 + current_page_line_index + 1, t);

	float gap = next_y_norm * scr_height / 2.0f;
	unsigned int tmp_li = current_line_index + 1;
	t->pos.y = texts[current_line_index]->pos.y;
	t->pos.y -= gap;

	current_line_index += 2;
	for(unsigned int i = current_line_index; i < texts.size(); i++)
		texts[i]->pos.y -= gap;

	current_line_index = tmp_li;
	cursor.pos.y -= next_y_norm;
	cursor.pos.x = box.pos.x - box.x_scale + 2.0f * text_gap_x_norm + cache_font_width_norm;
	current_char_index = 0;
}

void NewTextBox::scroll_down()
{
	page.pop_front();
	current_line_index++;
	page.push_back(texts[current_line_index]);

	float start_y = up_left_org.y + box_dims_org.y;
	start_y -= font_size;
	float gap = next_y_norm * scr_height / 2.0f;

	for(unsigned int i = 0; i < page.size(); i++)
	{
		Text *ft = page[i];
		ft->pos.y = start_y;	

		start_y -= gap;
	}

	if(current_char_index > texts[current_line_index]->text.size() - 1)
	{
		set_char_max();
	}
}

void NewTextBox::scroll_up()
{
	page.pop_back();
	current_line_index--;
	page.push_front(texts[current_line_index]);

	float start_y = up_left_org.y + box_dims_org.y;
	start_y -= font_size;
	float gap = next_y_norm * scr_height / 2.0f;

	for(unsigned int i = 0; i < page.size(); i++)
	{
		Text *ft = page[i];
		ft->pos.y = start_y;	

		start_y -= gap;
	}

	if(current_char_index > texts[current_line_index]->text.size() - 1)
	{
		set_char_max();
	}
}

void NewTextBox::save(std::string f_name)
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
		if(i != texts.size() - 1)
			wr << "\n";
	}

	std::cout << "saved: " << f_name << std::endl;
	wr.close();
}

void NewTextBox::load(std::string f_name)
{
	std::ifstream read;
	read.open(f_name);
	if(!read)
	{
		std::cout << "Could not open file: " << f_name << " for reading" << std::endl;
		return;
	}

	// Free the existing memory
	for(unsigned int i = 0; i < texts.size(); i++)
		free(texts[i]);	

	texts.clear();
	page.clear();

	std::cout << "loading..." << std::endl;
	while(!read.eof())
	{
		std::string line;
		if(!read.eof())
		{
			getline(read, line);
			add_text(line);
		}
	}

	std::cout << "loaded: " << f_name << std::endl;
	read.close();
}

void NewTextBox::insert(char ch)
{
	std::string tmp;
	tmp.push_back(ch);
	texts[current_line_index]->text.insert(current_char_index, tmp);

	current_char_index++;
	cursor.pos.x += cache_font_width_norm * 2.0f;
}

void NewTextBox::init(std::string font_name, unsigned int font_size, unsigned int w, unsigned int h)
{
	scr_width = w;
	scr_height = h;

	this->font_name = font_name;
	this->font_size = font_size;

	text_gap_x_norm = text_gap_x_org / (float)scr_width;
	text_gap_y_norm = text_gap_y_org / (float)scr_height;

	box.init();
	box.pos.x = -0.1f;
	box.pos.y = +0.1f;
	box.x_scale = 0.8f;
	box.y_scale = 0.8f;
	box.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	int tmp_x = get_org_x(box.pos.x, scr_width);
	int tmp_y = get_org_y(box.pos.y, scr_height);
	int box_w = (box.x_scale / 2.0f) * scr_width;
	int box_h = (box.y_scale / 2.0f) * scr_height;

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
	if(current_char_index == texts[current_line_index]->text.size())
		return;

	current_char_index++;
	cursor.pos.x += cache_font_width_norm * 2.0f;	
}

void NewTextBox::go_prev_char()
{
	if(current_char_index == 0)
		return;

	current_char_index--;
	cursor.pos.x -= cache_font_width_norm * 2.0f;	
}

void NewTextBox::go_next_line()
{
	if(current_line_index == texts.size() - 1)
		return;

	if(current_page_line_index >= max_lines_per_page - 1)
	{
		scroll_down();
		return;
	}
	else
		current_page_line_index++;

	current_line_index++;
	if(current_char_index > texts[current_line_index]->text.size() - 1)
		set_char_max();

	cursor.pos.y -= next_y_norm;
}

void NewTextBox::go_prev_line()
{
	if(current_line_index == 0)
		return;

	if(current_page_line_index > 0)
		current_page_line_index--;
	else
	{
		current_page_line_index = 0;
		scroll_up();
		return;
	}

	current_line_index--;
	if(current_char_index > texts[current_line_index]->text.size())
		set_char_max();

	cursor.pos.y += next_y_norm;
}

void NewTextBox::set_char_max()
{
	cursor.pos.x = box.pos.x - box.x_scale + 2.0f * text_gap_x_norm + cache_font_width_norm;
	current_char_index = 0;

	unsigned int _sz = texts[current_line_index]->text.size();
	for(unsigned int i = 0; i < _sz; i++)
		go_next_char();	
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

	if(texts.size() <= max_lines_per_page)
	{
		page.push_back(t);
	}

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

	if(texts.size() == 2)
	{
		next_y_norm = (texts[0]->pos.y - texts[1]->pos.y) / (float)scr_height;
		next_y_norm *= 2.0f;

		float tmp = (box_dims_org.y * 2 / (float)scr_height);
		tmp = tmp / (float)next_y_norm;
		tmp *= 2.0f;

		max_lines_per_page = (unsigned int)tmp;
	}
}

void NewTextBox::draw()
{
	box.draw();
	cursor.draw();

	for(unsigned int i = 0; i < page.size(); i++)
	{
		page[i]->font.render_text(page[i]->text, page[i]->pos.x, page[i]->pos.y, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
		// std::cout << i << " ";
	}
	// std::cout << std::endl;
}

#endif
/* A redo of text_box.h */
