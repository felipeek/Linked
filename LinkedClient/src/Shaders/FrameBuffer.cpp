#include "FrameBuffer.h"
#include "Entity.h"
#include "Camera.h"

#include <glm\glm.hpp>

FrameBuffer::FrameBuffer(int width, int height)
	: width(width), height(height)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	texture = new Texture(width, height);
	//shader = new FrameShader(DEPTH_SHADER_PATH);
	//mesh = new Mesh(new Quad(glm::vec3(0.65f, 0.65f, 0), 0.3f, 0.3f));
	////mesh = new Mesh(new Quad(glm::vec3(0.0f, 0.0f, 0), 1.0f, 1.0f));
	//e = new Entity(new Transform(glm::vec3(0, 0, 0)), mesh, t);
}

FrameBuffer::~FrameBuffer()
{
	if (texture != nullptr)
		delete texture;
	if (lightCamera != nullptr)
		delete lightCamera;
}

void FrameBuffer::genShadowMap(glm::vec4& borderColor)
{
	// Bind this texture to depth component of the framebuffer
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	// What color is going to be outside the shadow (1,1,1,1) recommended
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&borderColor[0]);

	// Bind frame buffer and link to this texture
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->textureID, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::genLightCamera(float orthoSpan)
{
	lightCamera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), orthoSpan, 15.0f, 140.0f);
}

void FrameBuffer::renderDepth()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);

	// render scene
}

void FrameBuffer::normalRender(int windowsWidth, int windowsHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowsWidth, windowsHeight);
	//e->render(shader);
}

GLuint FrameBuffer::genRenderBuffer(int width, int height)
{
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return rbo;
}

Texture* FrameBuffer::getTexture()
{
	return texture;
}

Camera* FrameBuffer::getCamera()
{
	return this->lightCamera;
}