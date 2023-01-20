#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void add_buffer(VertexBuffer& vb, VertexBufferLayout& layout);

	void bind();
	void unbind();
};