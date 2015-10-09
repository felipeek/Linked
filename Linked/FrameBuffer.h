#pragma once
#include <GL/glew.h>

class Texture;
class Shader;

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void renderPassToTexture(Shader* shader);
private:
	int width, height;
	GLuint frameBuffer;
	GLuint renderBuffer;

	GLuint texColorBuffer;
	GLuint genRenderBuffer(int width, int height);
};

