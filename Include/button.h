#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <glad/glad.h>
#include <font.h>
#include <rect.h>
#include <string>
#include <utils.h>

struct Button
{
	private:
		float rect_x = 0;
		float rect_y = 0;

		void set_text_pos_x(float x, int scr_width);
		void set_text_pos_y(float y, int scr_height);

	public:
		Rect rect;
		Font font;
		std::string text;
		glm::vec3 text_color = glm::vec3(1.0f, 1.0f, 1.0f);
		float text_pos_x = 0;
		float text_pos_y = 0;

		void update();
		void draw();
		void init(std::string text, int scr_width, int scr_height, const char* font_name, unsigned int _sz);
		void set_x(float x, int scr_width);
		void set_y(float y, int scr_height);
		glm::vec3 get_pos(unsigned int scr_width, unsigned int scr_height);
};

glm::vec3 Button::get_pos(unsigned int scr_width, unsigned int scr_height)
{
	float tmp_x = get_x_normalized(text_pos_x, scr_width);
	float tmp_y = get_y_normalized(text_pos_y, scr_height);
	return glm::vec3(tmp_x, tmp_y, 0.0f);
}

void Button::set_x(float x, int scr_width)
{
	rect.pos.x = x;
	set_text_pos_x(x, scr_width);
}

void Button::set_y(float y, int scr_height)
{
	rect.pos.y = y;
	set_text_pos_y(y, scr_height);
}

void Button::draw()
{
	rect.draw();
	font.render_text(text, text_pos_x, text_pos_y, 1.0f, text_color);
}

void Button::set_text_pos_x(float x, int scr_width)
{
	float norm_w = font.get_width(text) / (float)scr_width;
	x -= norm_w;

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

	text_pos_x = final_norm_x * scr_width;
}

void Button::set_text_pos_y(float y, int scr_height)
{
	float norm_h = font.get_height(text) / (float)scr_height;
	y -= norm_h / 2.0f;

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

	text_pos_y = scr_height - (final_norm_y * scr_height);
}

void Button::init(std::string text, int scr_width, int scr_height, const char* font_name, unsigned int _sz)
{
	this->text = text;
	rect_x = 0.0f;
	rect_y = 0.0f;

	font.init_lib();
	font.init_program(scr_width, scr_height);
	font.make_buffer();
	font.init_font(text, font_name, _sz);

	// rect default props
	rect.init();
	rect.color = glm::vec4(0.7f, 0.0f, 0.0f, 1.0f);
	rect.y_scale = 0.08f;

	float norm_w = font.get_width(text) / (float)scr_width;
	float norm_h = font.get_height(text) / (float)scr_height;
	rect.x_scale = norm_w + 0.02f;
	rect.y_scale = norm_h + 0.03f;

	set_text_pos_x(0.0, scr_width);
	set_text_pos_y(0.0, scr_height);
}

void Button::update()
{
	rect.update();
}

#endif
