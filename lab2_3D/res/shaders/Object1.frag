#version 330 core

in vec3 frag_pos;
in vec3 normal;
in vec4 color;

uniform vec3 u_LightPos;

out vec4 frag_color;

void main()
{
    float ambient_strength = 0.01;
    vec4 ambient = ambient_strength * vec4(1.0f, 1.0f, 0.1f, 1.0f);

    vec3 light_dir = normalize(u_LightPos - frag_pos);
    vec4 diffuse = max(dot(normal, light_dir), 0.0f) * vec4(1.0f, 1.0f, 1.0f, 1.0f);

    vec4 result = (ambient + diffuse) * color;
    frag_color = result;
};