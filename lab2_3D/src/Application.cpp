#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define WIDTH 600
#define HEIGHT 480

using namespace std;

glm::mat4 projection = glm::mat4(1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float camera_dist = 10.0f;
float lastFrame = 0.0f;

float surf_size = 5.0f;
float object_dist = 2.0f;
float fov = 45.0f;
float far_plane = 20.0f;
float near_plane = 0.1f;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

void key_callback(GLFWwindow* window);
void calc_surface(vector<Vertex>& vertices, vector<unsigned int>& indices, int, int);
void calc_normal(vector<Vertex>& vertices, vector<unsigned int>& indices);


int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello world!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		cout << "GLEW initialisation error!" << endl;

	{
		//-------------FUNCTION-SURFACE-------------//
		
		// function surface poistions and normals calculation
		vector<Vertex> func_vertices;
		vector<unsigned int> func_indices;
		calc_surface(func_vertices, func_indices, surf_size, surf_size);
		calc_normal(func_vertices, func_indices);
		unsigned int surface_vb_size = func_vertices.size() * sizeof(Vertex);
		unsigned int surface_ib_size = func_indices.size() * sizeof(unsigned int);

		//-----------------PYRAMIDS-----------------//

		// pyramids poistions and normals setting
		vector<Vertex> pyram_vertices;
		pyram_vertices.push_back({ glm::vec3(-0.5f, 0.0f, 0.5f) });
		pyram_vertices.push_back({ glm::vec3(0.0f, 0.5f,  0.0f) });
		pyram_vertices.push_back({ glm::vec3(0.5f, 0.0f,  0.5f) });
		pyram_vertices.push_back({ glm::vec3(0.0f, 0.0f, -0.5f) });

		// actual pyramid indices
		unsigned int initial_pyram_indices[] = { 0, 2, 1,   2, 3, 1,
												 3, 0, 2,   0, 3, 1 };

		vector<unsigned int> pyram_indices;
		for (auto i : initial_pyram_indices)
			pyram_indices.push_back(i);

		calc_normal(pyram_vertices, pyram_indices);

		// calculate indices for pyramid in vertex buffer
		for (int i = 0; i < pyram_indices.size(); i++)
			pyram_indices[i] += func_vertices.size();

		unsigned int pyram_vb_size = pyram_vertices.size() * sizeof(Vertex);
		unsigned int pyram_ib_size = pyram_indices.size() * sizeof(unsigned int);


		VertexBuffer vb((void*)0, surface_vb_size + pyram_vb_size);
		vb.addSubData(0, surface_vb_size, &func_vertices[0]);
		vb.addSubData(surface_vb_size, pyram_vb_size, &pyram_vertices[0]);

		VertexArray va;
		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(3);
		va.add_buffer(vb, layout);

		IndexBuffer ib((unsigned int*)0, surface_ib_size + pyram_ib_size);
		ib.addSubData(0, surface_ib_size, &func_indices[0]);
		ib.addSubData(surface_ib_size, pyram_ib_size, &pyram_indices[0]);

	
		//-----------------CAMERA-MVP-----------------//

		projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, near_plane, far_plane);

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -camera_dist);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model1 = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f, -object_dist, 0.0f));
		glm::mat4 model2 = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f,  object_dist, 0.0f));
		glm::mat4 model = glm::identity<glm::mat4>();
		model = glm::rotate(model, glm::radians(-90.0f),  glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 mvp = glm::mat4(1.0f);

		//-------------------SHADER-------------------//

		Shader shader("res/shaders/Object1.vert", "res/shaders/Object1.frag");

		glEnable(GL_DEPTH_TEST);

		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			key_callback(window);

			view = glm::lookAt(cameraFront, cameraTarget, cameraUp);

			va.bind();
			ib.bind();
			shader.bind();

			shader.set_uniform3f("u_LightPos", 1.0f, 2.0f, 5.0f);
			shader.set_uniform_matrix4fv("u_Projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader.set_uniform_matrix4fv("u_View", 1, GL_FALSE, glm::value_ptr(view));

			// draw function surface
			shader.set_uniform_matrix4fv("u_Model", 1, GL_FALSE, glm::value_ptr(model));
			shader.set_uniform4f("u_Color", 0.0f, 0.8f, 1.0f, 1.0f);
			glDrawElements(GL_TRIANGLES, func_indices.size(), GL_UNSIGNED_INT, (void*)0);

			// draw pyramids
			shader.set_uniform_matrix4fv("u_Model", 1, GL_FALSE, glm::value_ptr(model1));
			shader.set_uniform4f("u_Color", 0.5f, 0.2f, 1.0f, 1.0f);
			glDrawElements(GL_TRIANGLES, pyram_indices.size(), GL_UNSIGNED_INT, (void*)(func_indices.size() * sizeof(unsigned int)));

			shader.set_uniform_matrix4fv("u_Model", 1, GL_FALSE, glm::value_ptr(model2));
			shader.set_uniform4f("u_Color", 1.0f, 1.0f, 0.5f, 1.0f);
			glDrawElements(GL_TRIANGLES, pyram_indices.size(), GL_UNSIGNED_INT, (void*)(func_indices.size() * sizeof(unsigned int)));

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		//glDeleteBuffers(1, &vb);
		//glDeleteProgram(shader);

		//after terminate our openGL context is destroyed
		//but destructor of index/vertex buffer try to clean
		//all its stuff at the end of main
		//no context => glGetError returns an error
		// 
		//so delete before terminate (braces)
	}   // <----

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window)
{
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float currentFrame = static_cast<float>(glfwGetTime());
	float deltaTime = currentFrame - lastFrame;
	float sensitivity = 50.0f;
	float deltaX = static_cast<float>(1.0f * deltaTime);

	lastFrame = currentFrame;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		xoffset = deltaX;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		xoffset = -deltaX;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		yoffset = deltaX;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		yoffset = -deltaX;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// pretend camera from getting flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// calculate trajectory of camera
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = camera_dist * glm::normalize(front);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)	// press to show frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)	// press to show wholesome
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// calculate near and far plane for orthographic projection
	float aspect_ratio = (float)WIDTH / (float)HEIGHT;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)	// press to show in ortho mode
	{
		projection = glm::ortho(-(far_plane / 2.0f + camera_dist) * (float)tan(glm::radians(fov / 2.0f)) * aspect_ratio,
								 (far_plane / 2.0f + camera_dist) * (float)tan(glm::radians(fov / 2.0f)) * aspect_ratio,
								-(far_plane / 2.0f + camera_dist) * (float)tan(glm::radians(fov / 2.0f)),
								 (far_plane / 2.0f + camera_dist) * (float)tan(glm::radians(fov / 2.0f)),
								 near_plane, (far_plane / 2.0f + camera_dist));
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)	// press to show in perspective mode
		projection = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, (far_plane + camera_dist));
}

void calc_surface(vector<Vertex>& vertices, vector<unsigned int>& indices, int width, int height)
{
	int x_div = 20, y_div = 20;
	float dx = (float)width / x_div;
	float dy = (float)height / y_div;
	glm::vec3 temp;
	float x, y, z;

	for (int i = 0; i < x_div; i++)
	{
		for (int j = 0; j < y_div; j++)
		{
			x = -1 * (float)width/2 + dx * i;
			y = (float)height/2 - dy * j;
			temp.x = x;
			temp.y = y;

			z = 0;
			if (x < 0)
				x = 0;
			if (y < 0)
				y = 0;
			
			z = sqrt(x) + sqrt(y);
			temp.z = z;
			vertices.push_back({ temp });

			indices.push_back(i * (y_div + 1) + j);
			indices.push_back(i * (y_div + 1) + j + 1);
			indices.push_back((i + 1) * (y_div + 1) + j);

			indices.push_back((i + 1) * (y_div + 1) + j);
			indices.push_back(i * (y_div + 1) + j + 1);
			indices.push_back((i + 1) * (y_div + 1) + j + 1);
		}

		x = -1 * (float)width / 2 + dx * i;
		y = -1 * (float)height / 2;
		temp.x = x;
		temp.y = y;

		z = 0;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;

		z = sqrt(x) + sqrt(y);
		temp.z = z;
		vertices.push_back({ temp });
	}

	for (int i = 0; i <= y_div; i++)
	{
		x = (float)width / 2;
		y = (float)height / 2 - i*dy;
		temp.x = x;
		temp.y = y;

		z = 0;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;

		z = sqrt(x) + sqrt(y);
		temp.z = z;
		vertices.push_back({ temp });
	}
}

void calc_normal(vector<Vertex> &vertices, vector<unsigned int> &indices)
{
	glm::vec3 vector1, vector2, vector3;
	glm::vec3 cross_prod;
	float cross_prod_len;
	for (int i = 0; i < indices.size(); i += 3)
	{
		vector1 = vertices[indices[i]].position;
		vector2 = vertices[indices[i + 1]].position;
		vector3 = vertices[indices[i + 2]].position;

		cross_prod = glm::cross((vector2 - vector1), (vector3 - vector2));

		vertices[indices[i]].normal += cross_prod;
		vertices[indices[i + 1]].normal += cross_prod;
		vertices[indices[i + 2]].normal += cross_prod;

	}

	for (int i = 0; i < vertices.size(); i++)
		vertices[i].normal = glm::normalize(vertices[i].normal);
}