#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>

class Texture;
class FrameShader;
class Camera;

#define FRAME_SHADER_PATH "./shaders/frameshader"
#define DEPTH_SHADER_PATH "./shaders/framebuffer/depth"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void genShadowMap(glm::vec4& borderColor);
	void genLightCamera(float orthoSpan);

	void renderDepth();
	void normalRender(int windowsWidth, int windowsHeight);

	Texture* getTexture();
	Camera* getCamera();
private:
	int width, height;
	GLuint frameBuffer;
	GLuint renderBuffer;

	GLuint genRenderBuffer(int width, int height);

	FrameShader* shader;
	Texture* texture;
	Camera* lightCamera;
};

