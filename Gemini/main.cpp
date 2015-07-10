#include <iostream>
#include "display.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <GL\glew.h>
#include "mesh.h"
#include <string>

#define YAW rotation.y
#define PITCH rotation.x

int main(int argc, char* args[]){
	
	using namespace gemini;
	using namespace util;
	using namespace graphics;

	Display display(1200, 800, "Hello, World!");

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	const glm::vec3 worldup(0.0f, 1.0f, 0.0f);
	int mouseX = 0, mouseY = 0;
	glViewport(0, 0, display.getWidth(), display.getHeight());

	Shader shader("res/testShader.vs", "res/testShader.fs");

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

	GLuint count = 36;
	
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertecies, GL_STATIC_DRAW);

	delete vertecies;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position))); // Vertex
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal))); // Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color))); // Color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texcoord))); // Texcoord

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glm::mat4x4 projection = glm::perspective(70.0f, 1200.0f / 800.0f, 0.01f, 1000.0f);
	glm::mat4x4 view;
	glm::mat4x4 model = glm::mat4x4(1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 position(0.0f, 0.0f, -5.0f);
	glm::vec3 rotation(0.0f, 0.0f, 0.0f);

	float mouseSens = 0.001f;
	float speed = 0.01f;

	glm::vec3 front(0.0f, 0.0f, 1.0f);
	glm::vec3 right(1.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	const float pi = glm::pi<float>();

	unsigned int i = 0;

	glLineWidth(4.0f);

	while (!display.isCloseRequested()){
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);

		if (display.isKeyHold(SDLK_ESCAPE)) display.close();
		
		display.getMouseSpeed(mouseX, mouseY);

		PITCH += mouseY * mouseSens;
		YAW += mouseX * mouseSens;

		if (i++ % 500 == 0) {
			std::cout << "YAW  : " << YAW << std::endl;
			std::cout << "PITCH: " << PITCH << std::endl;
			std::cout << "FRONT: " << front.x << ", " << front.y << ", " << front.z << std::endl;
			std::cout << "RIGHT: " << right.x << ", " << right.y << ", " << right.z << std::endl;
		}

		front.x = cos(YAW - pi / 2.0f) * cos(-PITCH);
		front.y = sin(-PITCH);
		front.z = -sin(YAW + pi / 2.0f) * cos(PITCH);

		right = glm::cross(front, worldup);
		up = glm::cross(right, front);

		if (display.isKeyHold(SDLK_w)) position -= front * speed;
		if (display.isKeyHold(SDLK_a)) position += right * speed;
		if (display.isKeyHold(SDLK_s)) position += front * speed;
		if (display.isKeyHold(SDLK_d)) position -= right * speed;
		if (display.isKeyHold(SDLK_q)) position += up * speed;
		if (display.isKeyHold(SDLK_e)) position -= up * speed;

		view = glm::mat4x4(1.0f);
		view = glm::rotate(view, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, position);

		if (display.hasResized()) {
			std::cout << "RESIZE!!" << std::endl;
			glViewport(0, 0, display.getWidth(), display.getHeight());
			projection = glm::perspective(60.0f, (float)display.getWidth() / (float)display.getHeight() , 0.01f, 1000.0f);
		}


		model = glm::rotate(model, 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));
		
		shader.Bind();
		shader.SetUniformMat4("pr_matrix", projection);
		shader.SetUniformMat4("vw_matrix", view);
		shader.SetUniformMat4("md_matrix", model);
		
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		

#if 1
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

		shader.Unbind();

		display.update();

		GLenum error = glGetError();
		if (error != GL_NO_ERROR){
			std::cout << "OpenGL error: " << error << std::endl;
		}

	}

	return 0;
}