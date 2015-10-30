#include "TextRenderer.h"
#include "TextShader.h"
#include "Common.h"

TextRenderer::TextRenderer(Shader* shader, std::string fontName)
{
	this->shader = shader;

	// Gen bitmaps for all these sizes
	initRenderer(fontName, _12PX, chars12px);
	initRenderer(fontName, _14PX, chars14px);
	initRenderer(fontName, _16PX, chars16px);
	initRenderer(fontName, _18PX, chars18px);

	// gen vao
	genDynamicVAO();
}


TextRenderer::~TextRenderer()
{
	for (unsigned int i = 0, size = chars12px.size(); i < size; i++)
		chars12px.erase(chars12px.begin());
	for (unsigned int i = 0, size = chars14px.size(); i < size; i++)
		chars14px.erase(chars14px.begin());
	for (unsigned int i = 0, size = chars16px.size(); i < size; i++)
		chars16px.erase(chars16px.begin());
	for (unsigned int i = 0, size = chars18px.size(); i < size; i++)
		chars18px.erase(chars18px.begin());
}

void TextRenderer::initRenderer(std::string fontName, int fontPixelSize, std::map<unsigned int, Character>& charList)
{
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(ft, fontName.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, fontPixelSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (unsigned long c = 0; c < 256; c++)
	{
		// Load character glyph 
		FT_UInt glyph_index;

		glyph_index = FT_Get_Char_Index(face, c);

		int error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);	// FT_LOAD_RENDER
		if (error)
			continue;

		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
			continue;

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		charList.insert(std::pair<unsigned int, Character>(c, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

int indices[] = { 0, 1, 2, 3, 4, 5 };

void TextRenderer::genDynamicVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::renderText(unsigned char* text, int textSize, GLfloat x, GLfloat y, charPixelSize scale, glm::vec3 color)
{
	this->shader->useShader();
	glUniform3f(((TextShader*)shader)->getUniformTextColorLocation(), color.x, color.y, color.z);
	// Activate corresponding render state	
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	std::map<unsigned int, Character>* charsList;

	switch (scale)
	{
	case _12PX: charsList = &chars12px; break;
	case _14PX: charsList = &chars14px; break;
	case _16PX: charsList = &chars16px; break;
	case _18PX: charsList = &chars18px; break;
	default:
		charsList = &chars14px;
	}

	for (unsigned int c = 0; c < textSize; c++)
	{
		Character ch = (*charsList)[text[c]];

		GLfloat xpos = x + ch.Bearing.x * FONT_SIZE;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * FONT_SIZE;

		GLfloat w = ch.Size.x * FONT_SIZE;
		GLfloat h = ch.Size.y * FONT_SIZE;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },

				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		// Update shader after binding stuff
		this->shader->update();

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	
		// Render quad
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * FONT_SIZE; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	this->shader->stopShader();
}

Shader* TextRenderer::getShader()
{
	return this->shader;
}