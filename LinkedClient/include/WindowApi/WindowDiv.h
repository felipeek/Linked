#pragma once
#include "Button.h"
#include "Display.h"
#include "Label.h"
#include "Mesh.h"
#include "Texture.h"
#include "Window.h"
#include "WindowShader.h"

#include <vector>
#include <glm/glm.hpp>

class Mesh;

namespace linked
{
class Window;
class Label;
class Button;

class WindowDiv
{
public:
	// Methods
	WindowDiv(const Window& window, int width, int height, float margin, float padding,
		glm::vec2& position, glm::vec4& backGroundColor, unsigned int hints);
	~WindowDiv();
	void render();
	void update();
	bool isButtonHovered();

	// Getters and setters
	glm::vec2 getRelativePosition() const { return m_windowRelativePosition; }
	glm::vec2 getPosition() const { return this->m_position; }
	inline void setWindowRelativePosition(glm::vec2& relativePos) { this->m_windowRelativePosition = relativePos; }
	void setAbsolutePosition(glm::vec2& absolutePos);
	float getAlphaValue() const { return this->m_backGroundColor.a; }
	glm::vec2 getDivBasePosition(float xoffset, float yoffset) const;
	int getWidth() const{ return this->m_width; }
	int getHeight() const { return this->m_height; }
	bool m_render;
	const Window& getWindow()const{ return this->m_window; }
	glm::vec2 getScreenPosition() const;
	std::vector<Label*>& getLabels(){ return this->labels; }
	std::vector<Button*>& getButtons(){ return this->buttons; }
	void setBackgroundTexture(Texture* texture){ this->m_backgroundTexture = texture; }
	Texture* getBackgroundTexture(){ return this->m_backgroundTexture; }
private:
	// Data
	float m_margin, m_padding;
	int m_width, m_height;
	glm::vec2 m_windowRelativePosition;
	glm::vec2 m_position;		// Position in pixel coords
	glm::vec4 m_backGroundColor;
	const Window& m_window;

	Mesh* divMesh;
	Texture* m_backgroundTexture;

	std::vector<Label*> labels;
	std::vector<Button*> buttons;
	// hints
	bool m_anchor_left, m_anchor_right, m_anchor_top,
		m_anchor_bottom, m_float_left, m_float_right,
		m_float_top, m_float_bottom, m_center_y, m_center_x;
	
private:
	// Methods
	void handleDivHints(unsigned int hints);
};
}