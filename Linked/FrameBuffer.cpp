#include "FrameBuffer.h"
#include "Texture.h"
#include <iostream>
#include "Shader.h"


FrameBuffer::FrameBuffer(int width, int height)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	texColorBuffer = Texture::genGLNullTexture(width, height);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	renderBuffer = genRenderBuffer(width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void FrameBuffer::renderPassToTexture(Shader* shader)
{
	//// First pass
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
	//glEnable(GL_DEPTH_TEST);
	//DrawScene();
	//
	//// Second pass
	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//
	//screenShader.Use();
	//glBindVertexArray(quadVAO);
	//glDisable(GL_DEPTH_TEST);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);
}