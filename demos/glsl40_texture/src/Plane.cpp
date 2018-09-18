#include "Plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace std;

Plane::Plane(GLFWwindow* window, int size){
	this->size   = size;
	this->window = window;
	this->tiros.resize(10);

	projectionMatrix = glm::ortho(0, size, 0, size, -1, 1);

	try {
		shader.compileShader("shader/glsl40_texture.vert");
		shader.compileShader("shader/glsl40_texture.frag");

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	shader.printActiveAttribs();
	glEnable(GL_CULL_FACE);
}

void Plane::loadTextures(){
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->LoadTexture("..\\..\\resources\\textura-bf.bmp", 1);
}

void Plane::init(){
	genPlane();
	genBuffers();
	loadTextures();
}

void Plane::update(double deltaTime){
	shader.setUniform("MVP",        projectionMatrix);
	shader.setUniform("xMouse",     xMouse);
	shader.setUniform("yMouse",     yMouse);
	shader.setUniform("zoom",       zoom);
	shader.setUniform("ativo",      ativo);
	shader.setUniform("tiros",      tiros);	
	shader.setUniform("raioLuneta", raioLuneta);
}

void Plane::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLubyte *)NULL);
	glBindVertexArray(0);
}

void Plane::genBuffers(){

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(0);  // Vertex position -> layout 0 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, texUV.size() * sizeof(vec2), (GLvoid*)&texUV[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // texture coords -> layout 1 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Plane::resize(int x, int y){

}
 // SETERS (raio da luneta) - (zoom da arma) - (mira ativa) - (posição do mouse);
void Plane::setRaioLuneta(float raioLuneta)
{
	this->raioLuneta = raioLuneta;
}

void Plane::setZoom(float zoom)
{
	this->zoom = zoom;
}

void Plane::setAtivo(bool ativo)
{
	this->ativo = ativo;
}

void Plane::setMouse(double xMouse, double yMouse)
{
	this->xMouse = xMouse;
	this->yMouse = yMouse;
}


//ADICIONA O TIRO NA POSIÇÃO RECEBIDA PELO PARAMETRO
void Plane::addTiro(float xMouse, float yMouse, int ind)
{
	this->tiros[ind] = vec2(xMouse, yMouse);
}

void Plane::genPlane(){

	// v0 -- top left
	vertices.push_back(vec3(0, size, 0));
	texUV.push_back(vec2(0.0f, 1.0f));

	 //v1 -- bottom left
	vertices.push_back(vec3(0, 0, 0));
	texUV.push_back(vec2(0.0f, 0.0f));
	
	// v2 -- bottom right
	vertices.push_back(vec3(size, 0, 0));
	texUV.push_back(vec2(1.0f, 0.0f));
	
	//v3 -- top right
	vertices.push_back(vec3(size, size, 0));
	texUV.push_back(vec2(1.0f, 1.0f));

	// triangle 1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	//// triangle 2
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
}
