#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
{
	this->m_Stride = 0;
}

vector<VertexBufferElement>& VertexBufferLayout::get_elements()
{
	return this->m_Elements;
}

unsigned int VertexBufferLayout::get_stride()
{
	return this->m_Stride;
}