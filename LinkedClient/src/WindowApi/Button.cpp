#include "Button.h"
#include "WindowDiv.h"
#include "ContextWindow.h"

namespace linked
{
	int Button::mouseStatus;
	bool Button::clicked;

	Button::Button(const WindowDiv& div, Label* label, glm::vec2& position, int width, int height, glm::vec4& backgroundColor)
		:
		m_div(div),
		m_label(label),
		m_position(position),
		m_width(width), m_height(height),
		clickedCallback(nullptr)
	{
		label->setPosition(m_position + label->getPosition());
		m_buttonMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), (float)m_width, (float)m_height), true);
		
		m_backgroundTexture = nullptr;
		m_backgroundNormalTexture = m_backgroundTexture;
		m_backgroundHoveredTexture = m_backgroundTexture;
		m_backgroundHeldTexture = m_backgroundTexture;

		m_backgroundColor = backgroundColor;
		m_backgroundNormalColor = backgroundColor;
		m_backgroundHoveredColor = backgroundColor;
		m_backgroundHeldColor = backgroundColor;

		m_labelTextColor = label->getTextColor();
		m_labelNormalTextColor = m_labelTextColor;
		m_labelHoveredTextColor = m_labelTextColor;
		m_labelHeldTextColor = m_labelTextColor;
	}

	Button::Button(const WindowDiv& div, int width, int height)
		: Button(div, nullptr, glm::vec2(0, 0), width, height, glm::vec4(0, 0, 0, 1)){}

	Button::~Button()
	{
		if (m_buttonMesh)
			delete m_buttonMesh;
	}

	void Button::render()
	{
		m_label->render();
		isHovered();
	}

	void Button::update()
	{
		if (Button::clicked)
		{
			if (isHovered() && m_div.getWindow().isFocused())
			{
				if (clickedCallback)
					clickedCallback();
				Button::clicked = false;
			}
		}
		// Hovered
		if (isHovered() && m_div.getWindow().isFocused() && mouseStatus == 0)
		{
			m_backgroundColor = m_backgroundHoveredColor;
			m_backgroundTexture = m_backgroundHoveredTexture;
			m_label->setTextColor(m_labelHoveredTextColor);
		}
		// Held
		else if (isHovered() && m_div.getWindow().isFocused() && mouseStatus == 1)
		{
			m_backgroundTexture = m_backgroundHeldTexture;
			m_backgroundColor = m_backgroundHeldColor;
			m_label->setTextColor(m_labelHeldTextColor);
		}
		// Not hovered
		else
		{
			m_backgroundTexture = m_backgroundNormalTexture;
			m_backgroundColor = m_backgroundNormalColor;
			m_label->setTextColor(m_labelNormalTextColor);
		}
	}

	void Button::flush()
	{
		Button::clicked = false;
	}

	bool Button::isHovered() const
	{
		glm::vec2 cursorPosition = ContextWindow::getCurrent().getCursorPosition();

		glm::vec2 buttonPosition = getScreenPosition();
		
		if (cursorPosition.x >= buttonPosition.x && cursorPosition.x <= buttonPosition.x + m_width &&
			cursorPosition.y >= buttonPosition.y && cursorPosition.y <= buttonPosition.y + m_height)
		{
			return true;
		}
#if 1	// Font debug
		glUseProgram(0);

		glLineWidth(1.0);

		glBegin(GL_LINES);

		glColor3f(1, 0.5f, 0);

		glVertex3f(cursorPosition.x, cursorPosition.y, 0.0f);	// Vertical left debug line
		glVertex3f(cursorPosition.x + 0.1f, cursorPosition.y, 0.0f);

		glEnd();

#endif
		return false;
	}

	glm::vec2 Button::getScreenPosition() const
	{
		glm::vec2 divPosition = m_div.getScreenPosition();
		glm::vec2 buttonPosition = divPosition + m_position;
		return buttonPosition;
	}

	void Button::mouseCallback(int button, int action, int mods)
	{
		if (button == 0 && action == 1)
			mouseStatus = 1;
		if (mouseStatus == 1 && button == 0 && action == 0)
		{
			mouseStatus = 0;
			clicked = true;
		}
	}

	void Button::setClickedCallback(void(*callback)())
	{
		this->clickedCallback = callback;
	}
}