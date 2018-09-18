#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include "Scene.h"
#include <vector>
#include "glslprogram.h"
#include "TextureManager.h"

class Plane : public Scene {
public:
	Plane(GLFWwindow* window, int size = 1);

	void init();
	void update(double t);
	void render();
	void resize(int, int);
	void setMouse(double xMouse, double yMouse);
	void setZoom(float zoom);
	void setAtivo(bool ativo);
	void addTiro(float xMouse, float yMouse, int ind);
	void setRaioLuneta(float raioLuneta);

private:
	void genPlane();
	void genBuffers();
	void loadTextures();

	float raioLuneta = 0.159;
	float xMouse;
	float yMouse;
	float zoom = 1.0f;
	bool  ativo = false;
	std::vector<vec2> tiros;
	
	GLuint vaoID;
	int size;
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> texUV;
	std::vector<unsigned int> indices;

	GLSLProgram shader;
	GLFWwindow* window;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 modelViewMatrix;

	vec3 planePos;
};