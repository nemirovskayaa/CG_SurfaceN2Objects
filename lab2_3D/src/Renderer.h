#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(f) if (! (f)) __debugbreak();
#define glCall(f) glClearError;\
	f;\
	ASSERT(glLogCall (#f, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

class Renderer
{
private:

public:
};