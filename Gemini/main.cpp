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

#define WIDTH 1280
#define HEIGHT 720

#define YAW camera->m_rotation.y
#define PITCH camera->m_rotation.x

int main(int argc, char** argv){
	
	const float PI = glm::pi<float>();

	using namespace gemini;
	using namespace util;
	using namespace graphics;

	Display display(WIDTH, HEIGHT, "Hello, World!");

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	int mouseX = 0, mouseY = 0;
	glViewport(0, 0, display.getWidth(), display.getHeight());

	Shader shader("res/testShader.vs", "res/testShader.fs");
	Camera* camera = new Camera();
	Texture* texture = new Texture();
	texture->load("res/test.png");
	texture->unbind();

	GLuint m_VAO, m_VBO, m_IBO;

	Vertex* vertecies = new Vertex[8]{
		Vertex(glm::vec3(-.5f, -.5f, .5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(-.5f, .5f, .5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(.5f, .5f, .5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(.5f, -.5f, .5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(-.5f, -.5f, -.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(-.5f, .5f, -.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(.5f, .5f, -.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(.5f, -.5f, -.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f))
	};

	GLuint* indices = new GLuint[36] {
		0, 2, 1, 0, 3, 2,
		4, 3, 0, 4, 7, 3,
		4, 1, 5, 4, 0, 1,
		3, 6, 2, 3, 7, 6,
		1, 6, 5, 1, 2, 6,
		7, 5, 6, 7, 4, 5
	};

	unsigned int count = 36;
	unsigned int verteciesCount = 8;
	
	std::vector<Mesh> meshes;
	Mesh mesh(&shader);
	mesh.loadData(vertecies, verteciesCount, indices, count);

	delete vertecies;
	delete indices;

	srand(time(NULL));

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			Mesh newMesh = mesh.createInstance();
			newMesh.m_position.x = i;
			newMesh.m_position.z = j;
			newMesh.m_size = glm::vec3(0.2f, 0.2f, 0.2f);
			newMesh.m_rotation.y = glm::radians((float)(rand() % 360));
			meshes.push_back(newMesh);
		}
	}

	glm::mat4x4 projection = glm::perspective(70.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
	glm::mat4x4 view;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	float mouseSens = 0.001f;
	float speed = 0.01f;

	glLineWidth(4.0f);

	while (!display.isCloseRequested()){
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);

		if (display.isKeyHold(SDLK_ESCAPE)) display.close();
		if (display.isKeyHold(SDLK_F11)) display.maximizeWindow();

		display.getMouseSpeed(mouseX, mouseY);

		PITCH += mouseY * mouseSens;
		YAW += mouseX * mouseSens;

		if (display.isKeyHold(SDLK_w)) camera->moveFront(speed);
		if (display.isKeyHold(SDLK_a)) camera->moveRight(-speed);
		if (display.isKeyHold(SDLK_s)) camera->moveFront(-speed);
		if (display.isKeyHold(SDLK_d)) camera->moveRight(speed);
		if (display.isKeyHold(SDLK_q)) camera->moveUp(speed);
		if (display.isKeyHold(SDLK_e)) camera->moveUp(-speed);

		camera->update();
		view = camera->getViewMatrix();

		if (display.hasResized()) {
			std::cout << "RESIZE!!" << std::endl;
			std::cout << display.getWidth() << std::endl;
			glViewport(0, 0, display.getWidth(), display.getHeight());
			float aspect = (float)display.getWidth() / (float)display.getHeight();
			projection = glm::perspective(60.0f, aspect, 0.01f, 1000.0f);
		}
		
		texture->bind();
		for (Mesh m : meshes){
			m.render(projection, view);
		}
		texture->unbind();

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
		glVertex3f(0.0f, 0.0f, 10.0f);
		glEnd();
#endif

		display.update();


		GLenum error = glGetError();
		if (error != GL_NO_ERROR){
			std::cout << "OpenGL error: " << error << std::endl;
		}

	}

	return 0;
}