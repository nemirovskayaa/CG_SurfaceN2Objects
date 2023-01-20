#pragma once

#include <vector>
#include <GL/glew.h>

using namespace std;

struct VertexBufferElement
{
	unsigned int  type;
	unsigned int  count;
	unsigned char normalized;

	static unsigned int get_size_of_type(unsigned int type)
	{
		switch (type)
		{
		case GL_UNSIGNED_INT:
			return 4;
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout();

	template<typename T>
	void push(unsigned int count)
	{
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::get_size_of_type(GL_FLOAT) * count;
	}

	template<>
	void push<char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE) * count;
	}

	vector<VertexBufferElement>& get_elements();
	unsigned int get_stride();
};