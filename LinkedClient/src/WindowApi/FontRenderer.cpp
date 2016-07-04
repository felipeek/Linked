#include "FontRenderer.h"
#include "FontShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "ContextWindow.h"
#include "Primitive.h"

FontRenderer::FontRenderer(float fontSize, int textureQuality)
{
	atlas = ftgl::texture_atlas_new(textureQuality, textureQuality, 1);
	font = ftgl::texture_font_new_from_file(atlas, fontSize, "./fonts/consola.ttf");

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

int FontRenderer::renderText(const unsigned char* text, int textSize, const glm::vec2& position, float rightLimit, float bottomLimit, const glm::vec4 color, FontShader* shader)
{
	int fittingIndex = 0;

	shader->useShader();
	shader->activateAlphaBlend();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->id);

	float scaleX = (float)ContextWindow::getCurrent().getWidth();
	float scaleY = (float)ContextWindow::getCurrent().getHeight();

	float x = position.x;

	for (int i = 0; i < textSize; i++)
	{
		unsigned char c = text[i];
		ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, c);
		if (glyph != NULL)
		{
			IndexedModel * im = fontMesh->getQuad()->getIndexedModel();

			float x0 = x + glyph->offset_x / scaleX;
			float y0 = position.y + glyph->offset_y / scaleY;
			float x1 = x0 + glyph->width / scaleX;
			float y1 = y0 - glyph->height / scaleY;

			if (rightLimit <= x1)
			{
				fittingIndex = i;
				break;
			}

			im->positions[0] = glm::vec3(x0, y1, 0);
			im->positions[1] = glm::vec3(x0, y0, 0);
			im->positions[2] = glm::vec3(x1, y1, 0);
			im->positions[3] = glm::vec3(x1, y0, 0);

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
	shader->stopShader();
	shader->deactivateAlphaBlend();
	return fittingIndex;	// if 0 everything fit
}


// 
int FontRenderer::RenderText(std::string text, int xPos, int yPos, float pixelWidthLimit, const glm::vec4 color, FontShader* shader, bool wordFormat)
{
	float scaleX = (float)ContextWindow::getCurrent().getWidth();
	float scaleY = (float)ContextWindow::getCurrent().getHeight();

	float x = xPos / scaleX;
	float firstPos = (float)xPos;
	float limitX = (pixelWidthLimit + xPos) / scaleX;
	float yAdvance = 0;

	auto fit = [&text, pixelWidthLimit](int index, int xPos, int headStart, ftgl::texture_font_t* font)
	{
		float x = (float)xPos;
		for (; text[index + 1] != ' ' && text[index + 1] != 0; ++index)
		{
			unsigned char c = text[index];
			ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, c);
			if (index > 0)
			{
				float kerning = texture_glyph_get_kerning(glyph, text[index - 1]);
				x += kerning;
			}
			x += glyph->advance_x;
			if ((xPos + pixelWidthLimit) < x + headStart)
				return false;
		}
		return true;
	};

	shader->useShader();
	shader->activateAlphaBlend();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->id);

	for (unsigned int i = 0; i < text.size(); i++)
	{
		unsigned char c = text[i];
		ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, c);
		if (glyph != NULL)
		{
			IndexedModel * im = fontMesh->getQuad()->getIndexedModel();

			if (i > 0)
			{
				if (text[i - 1] == ' ' && text[i] != ' ')
				{
					if (!fit(i, firstPos, x * scaleX, font) && c != ' ')
					{
						x = xPos / scaleX;
						yPos -= fontSize;		// Note(Ho): Is this a bug?
						yAdvance++;
					}
				}
			}

			if (i > 0)
			{
				float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
				x += kerning / scaleX;
			}

			float x0 = x + glyph->offset_x / scaleX;
			float y0 = (yPos + glyph->offset_y) / scaleY;
			float x1 = x0 + glyph->width / scaleX;
			float y1 = y0 - glyph->height / scaleY;

			im->positions[0] = glm::vec3(x0, y1, 0);
			im->positions[1] = glm::vec3(x0, y0, 0);
			im->positions[2] = glm::vec3(x1, y1, 0);
			im->positions[3] = glm::vec3(x1, y0, 0);

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
	shader->stopShader();
	shader->deactivateAlphaBlend();
	return yAdvance;		// Note(Ho): Check this
}
