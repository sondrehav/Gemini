#include <iostream>
#include "display.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <GL\glew.h>
#include "mesh.h"
#include <string>
#include <vector>
#include <time.h>
#include "texture.h"
#include "camera.h"
#include "scene.h"
#include <time.h>

#define WIDTH 1280
#define HEIGHT 720

#define CLIP_NEAR 50.0f
#define CLIP_FAR 100000.0f

#define CAMERA_SPEED 100.0f

#define FRAME_RATE_CAP 60

int main(int argc, char** argv){
	
	const float PI = glm::pi<float>();

	using namespace gemini;
	using namespace util;
	using namespace graphics;

	Display display(WIDTH, HEIGHT, "Hello, World!");

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	int mouseX = 0, mouseY = 0;
	glViewport(0, 0, display.getWidth(), display.getHeight());

	Camera* camera = new Camera();
	
	Scene* scene = new Scene();
	
	if (!scene->loadScene("res/scene/sponza/sponza_edit.obj"));
	{
		std::cerr << "Could not load scene." << std::endl;
	}

	glm::mat4x4 projection = glm::perspective(70.0f, (float)WIDTH / (float)HEIGHT, CLIP_NEAR, CLIP_FAR);
	glm::mat4x4 view;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	float mouseSens = 0.001f;
	float speed = CAMERA_SPEED;

	glm::vec3 lightDirection(-0.5f, 0.5f, -0.5f);

	glLineWidth(4.0f);
	bool wf = false;
	std::cout << "Starting main loop..." << std::endl;
	float e = 0.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int frameCount = 0;

	while (!display.isCloseRequested()){

		Uint32 time = SDL_GetTicks();
		Uint32 time2 = time;

		//e += 0.003f;
		//lightDirection.x = cos(e) * 0.5;
		//lightDirection.z = sin(e) * 0.5;

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);

		if (display.isKeyDown(SDLK_F1))
		{
			wf = !wf;
			if (wf) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		if (display.isKeyHold(SDLK_ESCAPE)) display.close();
		if (display.isKeyHold(SDLK_F11)) display.maximizeWindow();

		display.getMouseSpeed(mouseX, mouseY);

		camera->m_rotation.x += mouseY * mouseSens;
		camera->m_rotation.y += mouseX * mouseSens;

		if (display.isKeyHold(SDLK_w)) camera->moveFront(speed);
		if (display.isKeyHold(SDLK_a)) camera->moveRight(-speed);
		if (display.isKeyHold(SDLK_s)) camera->moveFront(-speed);
		if (display.isKeyHold(SDLK_d)) camera->moveRight(speed);
		if (display.isKeyHold(SDLK_q)) camera->moveUp(speed);
		if (display.isKeyHold(SDLK_e)) camera->moveUp(-speed);

		if (display.isKeyHold(SDLK_l)) lightDirection = camera->front();

		camera->update();
		view = camera->getViewMatrix();


		//shader.SetUniform3f("lightDir", lightDirection.x, lightDirection.y, lightDirection.z);
		//shader.SetUniform3f("view_direction", camera->front().x, camera->front().y, camera->front().z);

		if (display.hasResized()) {
			std::cout << "RESIZE!!" << std::endl;
			std::cout << display.getWidth() << std::endl;
			glViewport(0, 0, display.getWidth(), display.getHeight());
			float aspect = (float)display.getWidth() / (float)display.getHeight();
			projection = glm::perspective(70.0f, aspect, CLIP_NEAR, CLIP_FAR);
		}
		
		scene->render(projection, camera->getViewMatrix(), lightDirection, camera->front());

#if 0
		shader.SetUniformMat4("md_matrix", glm::mat4x4(1.0f));
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
#endif

		display.update();

		time = SDL_GetTicks() - time; // Time elapsed
		Uint32 timeToWait = 1.0 / ((float)FRAME_RATE_CAP / 1000.0f) - time;
		if (timeToWait < 0)
		{
			SDL_Delay(timeToWait);
		}

		GLenum error = glGetError();
		if (error != GL_NO_ERROR){
			std::cout << "OpenGL error: " << error << std::endl;
		}

		time2 = SDL_GetTicks() - time2;
		float fps = 1000.0f / (float)time2;
		//if (frameCount++ % 60 == 0)
			//std::cout << fps << std::endl;

	}

	return 0;
}