#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;	// identifier for renderer
	unsigned int m_Count;		// count for indicies

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void addSubData(int offset, unsigned int count, const unsigned int* data);
	void bind();
	void unbind();
	unsigned int get_count();
};