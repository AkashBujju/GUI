#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <glad/glad.h>
#include <font.h>
#include <rect.h>
#include <string>

struct Button
{
private:
    Font font;
    std::string text;
    float text_pos_x = 0;
    float text_pos_y = 0;
    float rect_x = 0;
    float rect_y = 0;

    void set_text_pos_x(float x, int scr_width);
    void set_text_pos_y(float y, int scr_height);

public:
    Rect rect;

    void update();
    void draw();
    void init(std::string text, int scr_width, int scr_height, const char* font_name, unsigned int _sz);
    void set_x(float x, int scr_width);
    void set_y(float y, int scr_height);
};

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
    font.render_text(text, text_pos_x, text_pos_y, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
}

void Button::set_text_pos_x(float x, int scr_width)
{
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
    font.init_lib();
    font.init_program(scr_width, scr_height);
    font.make_buffer();
    font.init_font(font_name, _sz);

    // rect default props
    rect.init();
    rect.color = glm::vec4(0.7f, 0.0f, 0.0f, 1.0f);
    rect.x_scale = 0.1f;
    rect.y_scale = 0.1f;

    this->text = text;
    rect_x = 0.0f;
    rect_y = 0.0f;

    set_text_pos_x(0.0f, scr_width);
    set_text_pos_y(0.0, scr_height);
}

void Button::update()
{
    rect.update();
}

#endif