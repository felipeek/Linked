#pragma once
#include <GL/glew.h>

class Texture;
class FrameShader;
class Mesh;

#define FRAME_SHADER_PATH "./shaders/frameshader"
#define DEPTH_SHADER_PATH "./shaders/framebuffer/depth"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	FrameBuffer(int width, int height, bool shadow);
	~FrameBuffer();

	void renderPassOneToTexture();
	void renderPassTwoToTexture();

	void renderDepth();
	void normalRender();

	GLuint textureID;
private:
	int width, height;
	GLuint frameBuffer;
	GLuint renderBuffer;

	GLuint depthMap;
	GLuint texColorBuffer;
	GLuint genRenderBuffer(int width, int height);

	FrameShader* shader;
	Mesh* mesh;
};

