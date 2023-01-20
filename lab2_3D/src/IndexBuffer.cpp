#include "IndexBuffer.h"
#include "Renderer.h"

#include <GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
	this->m_Count = count;
	glCall(glGenBuffers(1, &m_RendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	glCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::addSubData(int offset, unsigned int count, const unsigned int* data)
{
	glCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(unsigned int), data));
}

void IndexBuffer::bind()
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind()
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::get_count()
{
	return this->m_Count;
}