#include "FontRenderer.h"
#include "FontShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "ContextWindow.h"
#include "Primitive.h"

FontRenderer::FontRenderer(float fontSize, int textureQuality)
{
	atlas = ftgl::texture_atlas_new(textureQuality, textureQuality, 1);
	font = ftgl::texture_font_new_from_file(atlas, fontSize, "./res/fonts/LiberationMono-Regular.ttf");

	ascender = font->ascender;
	this->fontSize = fontSize;
	fontMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0, 0), true);
}

FontRenderer::~FontRenderer()
{
	ftgl::texture_atlas_delete(atlas);
	ftgl::texture_font_delete(font);
	delete fontMesh;
}

TextInfo FontRenderer::RenderText(std::string text, float xPos, float yPos, float pixelWidthLimit, const glm::vec4 color, FontShader* shader, bool wordFormat)
{
	TextInfo result;
	result.start_position = glm::vec2(0, 0);
	result.end_position = glm::vec2(0, 0);

	float scaleX = ContextWindow::getCurrent().getWidth();
	float scaleY = ContextWindow::getCurrent().getHeight();

	float x = xPos / scaleX;
	float firstPos = (float)xPos;
	float limitX = (pixelWidthLimit + xPos) / scaleX;
	int yAdvance = 0;

	auto fit = [&text, pixelWidthLimit](int index, float xPos, float headStart, ftgl::texture_glyph_t* glyph)
	{
		float x = (float)xPos;
		for (; text[index + 1] != ' ' && text[index + 1] != 0; ++index)
		{
			unsigned char c = text[index];
			x += glyph->advance_x;
			if ((xPos + pixelWidthLimit) < x + headStart)
				return false;
		}
		return true;
	};

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->useShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->id);

	for (int i = 0; i < text.size(); i++)
	{
		unsigned char c = text[i];
		ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, c);
		if (glyph != NULL)
		{
			IndexedModel * im = fontMesh->getQuad()->getIndexedModel();
			if (i > 0)
			{
				float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
				x += kerning / scaleX;
			}

			if (i > 0)
			{
				if (text[i - 1] == ' ' && text[i] != ' ')
				{
					if (!fit(i, firstPos, x * scaleX, glyph) && c != ' ')
					{
						x = xPos / scaleX;
						yPos -= font->ascender - font->descender;	// highest glyph height - lowest glyph height
						yAdvance++;
					}
				}
			}

			float x0 = x + glyph->offset_x / scaleX;
			float y0 = (yPos + glyph->offset_y) / scaleY;
			float x1 = x0 + glyph->width / scaleX;
			float y1 = y0 - glyph->height / scaleY;

			if (i == 0)
			{
				result.start_position.x = x0;
				result.start_position.y = y0;
				result.end_position.x = x1;
			}
			if (x1 > result.end_position.x)
				result.end_position.x = x1;
			result.end_position.y = y1;
		
			im->positions[0] = glm::vec3(x0, y1, 0.0f);
			im->positions[1] = glm::vec3(x0, y0, 0.0f);
			im->positions[2] = glm::vec3(x1, y1, 0.0f);
			im->positions[3] = glm::vec3(x1, y0, 0.0f);

#if 0	// Font debug
			glUseProgram(0);

			glLineWidth(1.0);
			glBegin(GL_LINES);

			glColor3f(1, 0, 0);

			glVertex3f(x0, y1, -1.0f);	// Vertical left debug line
			glVertex3f(x0, y0, -1.0f);

			glVertex3f(x1, y1, -1.0f);	// Vertical right
			glVertex3f(x1, y0, -1.0f);

			glVertex3f(x0, y1, -1.0f);	// Horizontal bottom
			glVertex3f(x1, y1, -1.0f);

			glVertex3f(x0, y0, -1.0f);	// Horizontal top
			glVertex3f(x1, y0, -1.0f);
			glEnd();

			shader->useShader();
#endif
			im->texCoords[0] = glm::vec2(glyph->s0, glyph->t1);
			im->texCoords[1] = glm::vec2(glyph->s0, glyph->t0);
			im->texCoords[2] = glm::vec2(glyph->s1, glyph->t1);
			im->texCoords[3] = glm::vec2(glyph->s1, glyph->t0);

			glBindVertexArray(fontMesh->getVertexArrayID());
			glBindBuffer(GL_ARRAY_BUFFER, fontMesh->getVertexBufferID());
			glBufferSubData(GL_ARRAY_BUFFER, 0, im->positions.size() * sizeof(float) * 3, &im->positions[0]);

			glBindBuffer(GL_ARRAY_BUFFER, fontMesh->getTextureBufferID());
			glBufferSubData(GL_ARRAY_BUFFER, 0, im->texCoords.size() * sizeof(float) * 2, &im->texCoords[0]);

			shader->setTextColor(color);
			shader->update();
			fontMesh->render();

			x += glyph->advance_x / scaleX;
		}
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glUseProgram(0);

	result.num_rows = yAdvance + 1;
	result.height = (font->ascender - font->descender) * (float)yAdvance;
	result.width = result.end_position.x - result.start_position.x;

#if 1	// Font debug
	glUseProgram(0);

	glLineWidth(1.0);

	glBegin(GL_LINES);

	glColor3f(1, 0, 0);

	glVertex3f(result.start_position.x, result.start_position.y, -1.0f);	// Vertical left debug line
	glVertex3f(result.end_position.x, result.end_position.y, -1.0f);

	glEnd();

	shader->useShader();
#endif

	return result;
}