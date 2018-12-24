#ifndef MY_FONT_H
#define MY_FONT_H

#include <iostream>
#include <string>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <ft2build.h>

#include FT_FREETYPE_H

struct Character {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

struct FontBuffer {
	unsigned int vao;
	unsigned int vbo;
};

struct Font {
	std::map<GLchar, Character> characters;
	FontBuffer fb;
	unsigned int program = -1;
	FT_Face face;
	FT_Library ft;
	std::string current_text;
	GLfloat current_scale = 0;

	void init_lib();
	void init_program(int scr_width, int scr_height);
	void make_buffer();
	void init_font(std::string text, std::string fname, unsigned int _sz);
	void render_text(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	unsigned int get_width(std::string tx);
	unsigned int Font::get_height(std::string tx);
	~Font();
};

// x:w and y:h
glm::vec2 get_font_dims(std::string text, std::string font_name, unsigned int _sz, unsigned int scr_width, unsigned int scr_height)
{
	Font tmp_font;
	tmp_font.init_lib();
	tmp_font.init_program(scr_width, scr_height);
	tmp_font.make_buffer();
	tmp_font.init_font(text, font_name, _sz);

	glm::vec2 dims;
	dims.x = tmp_font.get_width(text);
	dims.y = tmp_font.get_height(text);

	return dims;
}

void init_font_props(Font &font, std::string text, std::string fname, unsigned int _sz)
{
	font.current_text = text;

	if (FT_New_Face(font.ft, fname.c_str(), 0, &font.face))
		std::cout << "Failed to load font: " << fname << std::endl;

	FT_Set_Pixel_Sizes(font.face, 0, _sz);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(font.face, c, FT_LOAD_RENDER))
		{
			std::cout << "Failed to load Glyph" << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				font.face->glyph->bitmap.width,
				font.face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				font.face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(font.face->glyph->bitmap.width, font.face->glyph->bitmap.rows),
			glm::ivec2(font.face->glyph->bitmap_left, font.face->glyph->bitmap_top),
			(int)font.face->glyph->advance.x
		};
		font.characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

Font::~Font()
{
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

unsigned int Font::get_width(std::string tx)
{
	unsigned int wi = 0;
	for (unsigned int i = 0; i < tx.size(); i++)
	{
		Character ch = characters[tx[i]];
		wi += ch.Advance / 64;
	}

	return wi;
}

unsigned int Font::get_height(std::string tx)
{
	unsigned int h = 0;
	for (unsigned int i = 0; i < tx.size(); i++)
	{
		Character ch = characters[tx[i]];
		if(ch.Size.y > h)
			h = ch.Size.y;
	}

	return h;
}

void Font::render_text(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	current_text = text;
	current_scale = scale;

	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fb.vao);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][4] = {
			{xpos, ypos + h, 0.0, 0.0},
			{xpos, ypos, 0.0, 1.0},
			{xpos + w, ypos, 1.0, 1.0},

			{xpos, ypos + h, 0.0, 0.0},
			{xpos + w, ypos, 1.0, 1.0},
			{xpos + w, ypos + h, 1.0, 0.0}};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, fb.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void Font::init_lib()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Could not init FreeType Library" << std::endl;
	}
}

void Font::init_font(std::string text, std::string fname, unsigned int _sz)
{
	init_font_props(*this, text, fname, _sz);
}

void Font::init_program(int scr_width, int scr_height)
{
	program = make_program("C:\\Users\\Akash\\Documents\\Programming\\OpenGL\\Include\\font_vertex",
			"C:\\Users\\Akash\\Documents\\Programming\\OpenGL\\Include\\font_fragment");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(scr_width), 0.0f,
			static_cast<GLfloat>(scr_height));
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Font::make_buffer()
{
	glGenVertexArrays(1, &fb.vao);
	glGenBuffers(1, &fb.vbo);
	glBindVertexArray(fb.vao);
	glBindBuffer(GL_ARRAY_BUFFER, fb.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
}

#endif
