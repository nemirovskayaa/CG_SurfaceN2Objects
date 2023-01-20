#include "Mesh.h"


Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, glm::vec4 color)
{
	this->vertices = vertices;
	this->indices = indices;
	this->color = color;
	this->model = glm::identity<glm::mat4>();
	this->model_translate = glm::identity<glm::mat4>();
	this->model_rotate = glm::identity<glm::mat4>();
	this->model_scale = glm::identity<glm::mat4>();
}

Mesh::~Mesh()
{
}

glm::mat4 Mesh::get_model()
{
	return this->model;
}

glm::mat4 Mesh::transate_to(glm::vec3 trans_vec)
{
	return this->model_translate = glm::translate(this->model_translate, trans_vec);
}

glm::mat4 Mesh::rotate_to(float angle, glm::vec3 axis)
{
	return this->model_rotate = glm::rotate(this->model_rotate, angle, axis);
}

glm::mat4 Mesh::scale_to(glm::vec3 scale_vec)
{
	return this->model_scale = glm::scale(this->model_scale, scale_vec);
}