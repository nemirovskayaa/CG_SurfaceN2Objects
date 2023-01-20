#version 330 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec4 u_Color;

out vec3 frag_pos;
out vec3 normal;
out vec4 color;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * aPosition;
	frag_pos = vec3(u_Model * aPosition);
	normal = aNormal;
	color = u_Color;
};