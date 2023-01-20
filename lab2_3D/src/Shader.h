#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class Shader
{
private:
	string m_vFilePath;
	string m_fFilePath;
	unsigned int m_RendererID;
	int get_uniform_location(const string& name);
	string parse_shader(const string& filepath);
	unsigned int create_shader(const string& vertex_shader_src, const string& fragment_shader_src);
	unsigned int compile_shader(const string& source, unsigned int type);

public:
	Shader(const string& v_filepath, const string& f_filepath);
	~Shader();

	void bind();
	void unbind();
	void set_uniform3f(const string& name, float v0, float v1, float v2);
	void set_uniform4f(const string& name, float v0, float v1, float v2, float v3);
	void set_uniform_matrix4fv(const string& name, int count, bool transpose, const float* value);
};