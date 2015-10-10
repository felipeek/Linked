#include "FrameBuffer.h"
#include "Texture.h"
#include <iostream>
#include "Shader.h"
#include "FrameShader.h"
#include <glm\glm.hpp>
#include "Mesh.h"
#include "Primitive.h"
#include "Entity.h"
Entity* e;
Texture* t;
FrameBuffer::FrameBuffer(int width, int height)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	// Attach it to currently bound framebuffer object
	t = new Texture(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t->textureID, 0);
	renderBuffer = genRenderBuffer(width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader = new FrameShader("./shaders/framebuffer/frameshader");
	mesh = new Mesh(new Quad(glm::vec3(0.65f, 0.65f, 0), 0.3f, 0.3f));
	
	e = new Entity(new Transform(glm::vec3(0, 0, 0)), mesh, t);
}


FrameBuffer::~FrameBuffer()
{
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

void FrameBuffer::renderPassOneToTexture()
{
	// First pass
	glBindFramebuffer(GL_FRAMEBUFFER, t->textureID);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
	glEnable(GL_DEPTH_TEST);
	// DRAW SCENE
}

void FrameBuffer::renderPassTwoToTexture()
{
	// Second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	e->render(shader);
}