#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

Shader::Shader(const string& v_filepath, const string& f_filepath)
{
	this->m_vFilePath = v_filepath;
	this->m_fFilePath = f_filepath;
	string vsource = parse_shader(v_filepath);
	string fsource = parse_shader(f_filepath);
	this->m_RendererID = create_shader(vsource, fsource);
}

Shader::~Shader()
{
	glCall(glDeleteProgram(m_RendererID));
}

string Shader::parse_shader(const string& filepath)
{
	string line;
	stringstream ss;
	ifstream stream(filepath);

	if (!stream)
		cout << "[ " << filepath << " ] : The file doesn't exist!" << endl;

	while (getline(stream, line))
		ss << line << '\n';

	return ss.str();
}

unsigned int Shader::create_shader(const string& vertex_shader_src, const string& fragment_shader_src)
{
	glCall(unsigned int program = glCreateProgram());
	unsigned int vshader = compile_shader(vertex_shader_src, GL_VERTEX_SHADER);
	unsigned int fshader = compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER);

	glCall(glAttachShader(program, vshader));
	glCall(glAttachShader(program, fshader));
	glCall(glLinkProgram(program));
	glCall(glValidateProgram(program));

	glCall(glDeleteShader(vshader));
	glCall(glDeleteShader(fshader));

	return program;
}

unsigned int Shader::compile_shader(const string& source, unsigned int type)
{
	const char* src = source.c_str();
	glCall(unsigned int id = glCreateShader(type));
	glCall(glShaderSource(id, 1, &src, nullptr));
	glCall(glCompileShader(id));

	int result;
	glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		glCall(glGetShaderInfoLog(id, length, &length, message));

		cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << endl;
		cout << message << endl;

		glCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

void Shader::bind()
{
	glCall(glUseProgram(m_RendererID));
}

void Shader::unbind()
{
	glCall(glUseProgram(0));
}

void Shader::set_uniform3f(const string& name, float v0, float v1, float v2)
{
	glCall(glUniform3f(get_uniform_location(name), v0, v1, v2));
}

void Shader::set_uniform4f(const string& name, float v0, float v1, float v2, float v3)
{
	glCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

void Shader::set_uniform_matrix4fv(const string& name, int count, bool transpose, const float* value)
{
	glCall(glUniformMatrix4fv(get_uniform_location(name), count, transpose, value));
}

int Shader::get_uniform_location(const string& name)
{
	glCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		cout << "Warning: uniform '" << name << "' doesn't exist!" << endl;

	return location;
}