#include "VertexBuffer.h"
#include "Renderer.h"
#include <GL/glew.h>

#include <iostream>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glCall(glGenBuffers(1, &m_RendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	glCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::addSubData(int offset, unsigned int size, const void* data)
{
	glCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::bind()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}