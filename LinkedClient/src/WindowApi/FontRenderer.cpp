#include "FontRenderer.h"
#include "FontShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Display.h"
#include "Primitive.h"

FontRenderer::FontRenderer(float fontSize, int textureQuality)
{
	atlas = ftgl::texture_atlas_new(textureQuality, textureQuality, 1);
	font = ftgl::texture_font_new_from_file(atlas, fontSize, "./fonts/arial.ttf");

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

int FontRenderer::renderText(const unsigned char* text, int textSize, const glm::vec2& position, float rightLimit, float bottomLimit,  const glm::vec4 color, FontShader* shader)
{
	int fittingIndex = 0;

	shader->useShader();
	shader->activateAlphaBlend();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->id);

	float scaleX = (float)Display::getCurrentInstance().getWidth();
	float scaleY = (float)Display::getCurrentInstance().getHeight();;

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

			float u0 = glyph->s0;
			float v0 = glyph->t0;
			float u1 = glyph->s1;
			float v1 = glyph->t1;

			im->positions[0] = glm::vec3(x0, y1, 0);
			im->positions[1] = glm::vec3(x0, y0, 0);
			im->positions[2] = glm::vec3(x1, y1, 0);
			im->positions[3] = glm::vec3(x1, y0, 0);

			im->texCoords[0] = glm::vec2(u0, v1);
			im->texCoords[1] = glm::vec2(u0, v0);
			im->texCoords[2] = glm::vec2(u1, v1);
			im->texCoords[3] = glm::vec2(u1, v0);

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
