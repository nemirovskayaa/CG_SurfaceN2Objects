#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;	// identifier for renderer

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void addSubData(int offset, unsigned int size, const void* data);
	void bind();
	void unbind();
};