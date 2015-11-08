#include "WindowDiv.h"

namespace linked
{
	WindowDiv::WindowDiv(
		const Window& window, 
		int width, int height,
		float margin, float padding,
		glm::vec2& position,
		glm::vec4& backGroundColor, 
		unsigned int hints) :
		m_window(window), 
		m_margin(margin), 
		m_padding(padding),
		m_width(width), 
		m_height(height), 
		m_position(position), 
		m_backGroundColor(backGroundColor), 
		m_render(true),
		m_backgroundTexture(nullptr)
	{
		handleDivHints(hints);
		divMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), (float)width, (float)height), true);
	}

	void WindowDiv::render()
	{
		WindowShader* ws = m_window.getWindowShader();
		ws->useShader();
		ws->activateAlphaBlend();
		if (m_render)
		{
			// Clips to edge of window and sets the correct blending mode to render divs
			ws->clipTL = m_window.getWindowBasePosition(0, 0);
			ws->clipBR = m_window.getWindowBasePosition((float)m_window.getWidth(), (float)m_window.getHeight());
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
			// Prepare the div and render it
			ws->setTextColor(m_backGroundColor);
			if (m_backgroundTexture != nullptr)
				ws->bindTextures(m_backgroundTexture->textureID);
		
			ws->update(getRelativePosition());
			divMesh->render();
		
			if (m_backgroundTexture != nullptr)
				ws->unbindTextures();
		}
		for (Button* b : buttons)
		{
			glm::vec2 v = getRelativePosition();
			v.x -= ((this->m_width - b->getWidth()) / 2.0f) - b->getPosition().x;
			v.y -= ((this->m_height - b->getHeight()) / 2.0f) - b->getPosition().y;
			ws->setTextColor(b->getBackgroundColor());
			if (b->getBackgroundTexture() != nullptr)
				ws->bindTextures(b->getBackgroundTexture()->textureID);
		
			ws->update(v);
			b->getButtonMesh().render();
		
			if (b->getBackgroundTexture() != nullptr)
				ws->unbindTextures();
		}
		ws->deactivateAlphaBlend();
		ws->stopShader();

		// Clip planes where the window ends
		Window::m_textShader->useShader();
		glm::vec2 w_clipTL = m_window.getWindowBasePosition(0, 0);
		glm::vec2 w_clipBR = m_window.getWindowBasePosition((float)m_window.getWidth(), (float)m_window.getHeight());
		
		Window::m_textShader->clipTL = w_clipTL;
		Window::m_textShader->clipBR = w_clipBR;
		
		Window::m_textShader->update();

		// Render all button labels
		for (Button* b : buttons)
			b->render();

		// Render all labels in order of creation
		for (Label* l : labels)
			l->render();

		ws->clipTL = glm::vec2(-1, -1);
		ws->clipBR = glm::vec2((float)Display::getCurrentInstance().getWidth(), (float)Display::getCurrentInstance().getHeight());

		Window::m_textShader->stopShader();
		
	}

	void WindowDiv::update()
	{
		setAbsolutePosition(m_position);
		for (Button* b : buttons)
			b->update();
	}

	void WindowDiv::setAbsolutePosition(glm::vec2& absolutePos)
	{
		if (m_anchor_left || m_anchor_right || m_anchor_top || m_anchor_bottom || m_center_x || m_center_y)
		{
			glm::vec2 result = glm::vec2(m_position.x, m_position.y);

			// Handle anchoring
			if (m_anchor_left)
				result.x -= (m_window.getWidth() - m_width - m_margin) / 2.0f - m_window.getPosition().x;
			else if (m_anchor_right)
				result.x -= (m_width - m_window.getWidth() + m_margin) / 2.0f - m_window.getPosition().x;

			if (m_anchor_top)
				result.y -= (m_window.getHeight() - m_height - m_margin) / 2.0f - m_window.getPosition().y;
			else if (m_anchor_bottom)
				result.y -= (m_height - m_window.getHeight() + m_margin) / 2.0f - m_window.getPosition().y;
			if (m_center_x)
				result.x = m_window.getPosition().x;
			if (m_center_y)
				result.y = m_window.getPosition().y;
			m_windowRelativePosition = result;
		}
		else
		{
			m_windowRelativePosition = m_position;
		}

	}

	WindowDiv::~WindowDiv()
	{
		if (divMesh)
			delete divMesh;
		if (m_backgroundTexture)
			delete m_backgroundTexture;

		for (Label* l : labels)
		{
			delete l;
			l = nullptr;
		}
		for (Button* b : buttons)
		{
			delete b;
			b = nullptr;
		}
	}

	void WindowDiv::handleDivHints(unsigned int hints)
	{
		m_anchor_left = (hints & DIV_ANCHOR_LEFT) ? true : false;
		m_anchor_right = (hints & DIV_ANCHOR_RIGHT) ? true : false;
		m_anchor_top = (hints & DIV_ANCHOR_TOP) ? true : false;
		m_anchor_bottom = (hints & DIV_ANCHOR_BOTTOM) ? true : false;
		m_float_left = (hints & DIV_FLOAT_LEFT) ? true : false;
		m_float_right = (hints & DIV_FLOAT_RIGHT) ? true : false;
		m_float_top = (hints & DIV_FLOAT_TOP) ? true : false;
		m_float_bottom = (hints & DIV_FLOAT_BOTTOM) ? true : false;
		m_center_y = (hints & DIV_CENTER_Y) ? true : false;
		m_center_x = (hints & DIV_CENTER_X) ? true : false;
	}

	glm::vec2 WindowDiv::getDivBasePosition(float xoffset, float yoffset) const
	{
		glm::vec2 basePos = m_windowRelativePosition;
		basePos.x += xoffset;
		basePos.y += yoffset;
		basePos.x -= m_width / 2.0f;
		basePos.y -= m_height / 2.0f;

		basePos.x /= Display::getCurrentInstance().getWidth() / 2.0f;
		basePos.y /= Display::getCurrentInstance().getHeight() / 2.0f;

		return basePos;
	}

	glm::vec2 WindowDiv::getScreenPosition() const
	{
		glm::vec2 divPosition = m_windowRelativePosition + glm::vec2(
			Display::getCurrentInstance().getWidth() / 2.0f,
			Display::getCurrentInstance().getHeight() / 2.0f);
		divPosition.x -= m_width / 2.0f;
		divPosition.y -= m_height / 2.0f;
		return divPosition;
	}

	bool WindowDiv::isButtonHovered()
	{
		for (Button* b : buttons)
		{
			if (b->isHovered())
				return true;
		}
		return false;
	}
}

