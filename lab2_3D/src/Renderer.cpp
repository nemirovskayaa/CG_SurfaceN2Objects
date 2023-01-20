#include "Renderer.h"

#include <iostream>

using namespace std;

void glClearError()
{
	while (glGetError() != GL_NO_ERROR); // clear all errors occured before
}

bool glLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) // output occured error
	{
		cout << "[OpenGl Error] (" << error << "): " << function << endl;
		cout << file << ":" << line << endl << endl;

		return false;
	}
	return true;
}