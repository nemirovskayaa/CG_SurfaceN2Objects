#include "VertexArray.h"

VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::add_buffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
	bind();
	vb.bind();

	unsigned int offset = 0;
	vector<VertexBufferElement> elements = layout.get_elements();
	VertexBufferElement element;
	for (int i = 0; i < elements.size(); i++)
	{
		element = elements[i];
		glCall(glEnableVertexAttribArray(i));
		glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void*)offset));

		offset += element.count * VertexBufferElement::get_size_of_type(element.type);
	}
}

void VertexArray::bind()
{
	glCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind()
{
	glCall(glBindVertexArray(0));
}