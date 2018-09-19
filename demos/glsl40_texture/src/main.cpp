#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

#include "Scene.h"
#include "Plane.h"

#define WINDOW_WIDTH	1350
#define WINDOW_HEIGHT	1000

using namespace std;

GLFWwindow* window;
Plane *plane;

int counTiros = 0;

float zoom = 1.0f;
float raioLuneta = 0.159f;

float time = 0.0f;
float incremento = 0.05;
float balance = 0.0;
float recoil = 0.007f;

float X, Y;

bool ativo = false;
bool respirando = true;

char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY) (glfwGetKey(WINDOW, KEY) ? (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) : (keyOnce[KEY] = false))

void mainLoop()
{
	double thisTime;
	double lastTime = glfwGetTime();
	do
	{

		//AUMENTA O RAIO DA LUNETA
		if (glfwGetKeyOnce(window, 'P'))
		{
			if (raioLuneta < 0.3f)
			{
				raioLuneta += 0.01f;
				plane->setRaioLuneta(raioLuneta);
			}
		}

		//REDUZ O RAIO DA LUNETA
		if (glfwGetKeyOnce(window, 'O'))
		{
			if (raioLuneta > 0.05f)
			{
				raioLuneta -= 0.01f;
				plane->setRaioLuneta(raioLuneta);
			}
		}

		//TRAVA A RESPIRAÇÃO - A MIRA FICA PARADA
		if (glfwGetKeyOnce(window, 'R'))
		{
			if (respirando)
				respirando = false;
			else
				respirando = true;
		}		

		// REALIZA ANIMAÇÃO DE RECOIL DO TIRO
		if (recoil > 0.29f)
		{
			respirando = true; //APÓS DAR O TIRO, VOLTA A RESPIRAR
			if (time > 0.0f)
				recoil = 0.007;
		}

		//MANTER O MOVIMENTO ENTRE -PI E +PI ----- FAZER A IDA E A VOLTA DO MOVIMENTO
		if (time > 3.1415f || time < -3.1415f)
			incremento = -incremento;


		// SE NÃO ESTIVER RESPIRANDO, NÃO REALIZA O INCREMENTO NAS VARIAVEIS DE MOVIMENTO
		if (respirando)
		{
			if (incremento < 0.0f)
				balance = sin(time) * 0.007f;
			else
				balance = sin(-time) * recoil;

			time += incremento;
		}

		// SETA AS COORDENADAS DO MOUSE + MOVIMENTO
		plane->setMouse(X + time * 0.00314, Y + balance);

		thisTime = glfwGetTime();
		plane->update(thisTime - lastTime);
		lastTime = thisTime;

		plane->render();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (!glfwWindowShouldClose(window));
}

void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void initGLFW()
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PRESS: (R) - Parar tremor, (P) - Aumentar tamanho da luneta, (O) - Diminuir o tamanho da luneta", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		system("pause");
		exit(EXIT_FAILURE);
	}
}

//CALLBACK DE MOVIMENTO DO MOUSE (SETA AS VARIAVEIS X E Y LIMITADOS ENTRE 0 E 1)
void MousePassiveMotion(GLFWwindow *window, double xMouse, double yMouse) 
{
	X = xMouse / WINDOW_WIDTH;
	Y = (WINDOW_HEIGHT - yMouse) / WINDOW_HEIGHT;
}

//CALLBACK DO SCROOL DO MOUSE   (VARIA O ZOOM DA ARMA ENTRE 0.1 E 1)
void MouseWheel(GLFWwindow *windows, double click, double dir)
{	
	if (dir == 1.0f  && zoom > 0.1F)
		zoom -= 0.1f;
	if (dir == -1.0f && zoom < 1.0F)
		zoom += 0.1f;

	plane->setZoom(zoom);
}

//CALBACK DE CLICK DO MOUSE
void MouseButton(GLFWwindow *window, int button, int click, int NAOSEIOQUEE)
{
	if (click == 1)
	{
		switch (button) {
			case 0:   //SÓ PODE ATIRAR SE A MIRA ESTIVER ATIVA
				if(ativo) 
				{
					plane->addTiro(X, Y, counTiros++ % 10); // %10 PARA LIMITAR O NUMERO DE TIROS NA TELA - INDICA O INDICE DO VETOR QUE SERÁ ADICIONADO O TIRO
					time = -3.1415f;             // SETA AS VARIAVEIS PARA REALIZAR A ANIMAÇÃO DE RECOIL DA ARMA
					if (incremento > 0)   
						incremento = -incremento;
					recoil = 0.3f;
				}
				break;

			case 1: // ATIVA E DESATIVA A MIRA DA ARMA
				ativo = !ativo;
				plane->setAtivo(ativo);
				break;
		}
	}
}


void initCallbacks()
{
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetErrorCallback(errorCallback);
	glfwSetCursorPosCallback(window, MousePassiveMotion);
	glfwSetMouseButtonCallback(window, MouseButton);
	glfwSetScrollCallback(window, MouseWheel);
}

void initGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		system("pause");
		exit(EXIT_FAILURE);
	}
}
void initializeGL()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void closeApplication()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(void)
{
	initGLFW();
	initCallbacks();
	initGLEW();
	initializeGL();

	plane = new Plane(window, 1);
	plane->init();

	cout << "'R' - Parar tremor da mira!" << endl;
	cout << "'P' - Aumentar tamanho da luneta!" << endl;
	cout << "'O' - Diminuir o tamanho da luneta!" << endl;


	mainLoop();

	exit(EXIT_SUCCESS);
}