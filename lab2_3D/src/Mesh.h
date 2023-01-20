#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

class Mesh
{
private:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	glm::mat4 model;
	glm::mat4 model_translate;
	glm::mat4 model_rotate;
	glm::mat4 model_scale;
	glm::vec4 color;


public:

	Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, glm::vec4 color);
	~Mesh();

	glm::mat4 get_model();
	glm::mat4 transate_to(glm::vec3 trans_vec);
	glm::mat4 rotate_to(float angle, glm::vec3 axis);
	glm::mat4 scale_to(glm::vec3 scale_vec);
};
