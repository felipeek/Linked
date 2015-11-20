#pragma once
#include "Primitive.h"
#include "Display.h"
#include "WindowShader.h"
#include "FontShader.h"
#include "FontRenderer.h"
#include "WindowDiv.h"
#include "Label.h"
#include "Mesh.h"
#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>

class Shader;
class WindowShader;
class Mesh;
class FontRenderer;

namespace linked
{
class WindowDiv;
class Label;

enum windowHints{
	W_RESIZEABLE		= 0x01,
	W_MOVABLE			= 0x02,
	W_HEADER			= 0x04,
	W_CLOSE_BUTTON		= 0x08,
	W_SCROLLBAR			= 0x10,
	W_BORDER			= 0x20
};

enum divHints{
	DIV_ANCHOR_LEFT		= 0x01,
	DIV_ANCHOR_RIGHT	= 0x02,
	DIV_ANCHOR_TOP		= 0x04,
	DIV_ANCHOR_BOTTOM	= 0x08,
	DIV_FLOAT_LEFT		= 0x10,
	DIV_FLOAT_RIGHT		= 0x20,
	DIV_FLOAT_TOP		= 0x40,
	DIV_FLOAT_BOTTOM	= 0x80,
	DIV_CENTER_Y		= 0x100,
	DIV_CENTER_X		= 0x200
};

class Window
{
#define WINDOW_SHADER_PATH "./shaders/windowapi/window_shader"
#define FONT_SHADER_PATH "./shaders/windowapi/font_shader"
#define DEFAULT_TITLE_COLOR 0.2f, 0.2f, 0.2f, 1.0f
#define DEFAULT_BORDER_SIZE 2		// pixels
#define DEFAULT_BORDER_COLOR 0.0f, 0.0f, 0.0f, 1.0f
#define DEFAULT_HEADER_SIZE 20.0f	// pixels
public: 
	// Static
	// Methods
	static void renderWindows();
	static void updateWindows();
	// Data
	static std::vector<Window*> openedWindows;
	static WindowShader* m_windowShader;
	static FontShader* m_textShader;
	static void linkedWindowDestroy();
	static void linkedWindowInit();
public: 
	// Methods
	Window(
		int width, 
		int height, 
		const glm::vec2& position,
		const glm::vec4& backgroundColor, 
		unsigned char* title, 
		int titleLength,
		unsigned int hints);
	~Window();
	unsigned int createDiv(
		const glm::vec2& relativePos,
		divHints hints,
		float defaultPadding,
		float defaultMargin);
	void render();
	void update();
		
	void mouseCallback(int button, int action, int mods);
private: 
	// Data
	int m_width, m_height;
	int m_titleLength;
	bool m_titleCentered;
	unsigned char* m_title;
	glm::vec4 m_titleColor;
	glm::vec2 m_position;
	glm::vec2 m_screenPosition;
	glm::vec4 m_backgroundColor;
	glm::vec4 m_borderColor;
	float m_borderSizeX, m_borderSizeY;
	float m_headerSize;
	bool m_attached;					// Mouse related
	glm::vec2 m_cursorPosWhenAttached;	// -
	glm::vec2 m_posWhenAttached;		// -

	bool h_resizeable, h_movable, h_header, h_closeButton,	// hints
		h_scrollbar, h_border;								// -
	bool focused;
	Mesh* m_windowMesh, *m_borderMesh;
	FontRenderer* m_fontRenderer;
public:
	std::vector<WindowDiv*> divs;

private: 
	// Methods
	void attachMouse();
	void detachMouse();
	void resize(const int width, const int height);
	void createWindowMesh();
	bool isHovered();
	bool isFocused();
	void handleWindowHints(unsigned int hints);		// hints related
	void setupBorder();

public: // Getters and Setters
	inline glm::vec2 getPosition() const { return this->m_position; }
	inline glm::vec2 getScreenPosition() const { return this->m_screenPosition; }
	void setPosition(const glm::vec2& newPosition);

	inline int getWidth()const{ return this->m_width; }
	inline int getHeight()const{ return this->m_height; }

	inline glm::vec4 getBackGroundColor() const { return this->m_backgroundColor; }
	inline void setBackGroundColor(const glm::vec4& newBackGroundColor) { this->m_backgroundColor = newBackGroundColor; }
	
	inline float getBorderSizeX() const{ return this->m_borderSizeX; }
	void setBorderSizeX(float newBorderSize);
	inline float getBorderSizeY() const{ return this->m_borderSizeY; }
	void setBorderSizeY(float newBorderSize);
	
	inline void setBorderColor(glm::vec4& newBorderColor){ this->m_borderColor = newBorderColor; }
	inline glm::vec4 getBorderColor()const{ return this->m_borderColor; }
	
	void setTitle(unsigned char* title, int titleLength);
	inline void setTitleColor(const glm::vec4& titleColor){ m_titleColor = titleColor; }
	
	inline bool isFocused() const{ return focused; }

	inline void setTitleCentered(bool value){ this->m_titleCentered = value; }

	glm::vec2 getWindowBasePosition(float xoffset, float yoffset) const;
	WindowShader* getWindowShader()const{ return m_windowShader; }
};
}

