#pragma once
#include <GL/glew.h>

class Texture;
class FrameShader;
class Mesh;

#define FRAME_SHADER_PATH "./shaders/frameshader"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void renderPassOneToTexture();
	void renderPassTwoToTexture();
private:
	int width, height;
	GLuint frameBuffer;
	GLuint renderBuffer;

	GLuint texColorBuffer;
	GLuint genRenderBuffer(int width, int height);

	FrameShader* shader;
	Mesh* mesh;
};

