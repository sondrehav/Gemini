#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "mesh.h"

namespace gemini { namespace graphics{

	Mesh::Mesh(Material* material, const Vertex *vertecies, unsigned int vertCount, const GLuint *indices, unsigned int indicesCount)
	{
		m_material = material;
		setData(vertecies, vertCount, indices, indicesCount);
	}

	Mesh::~Mesh()
	{

	}

	bool Mesh::setData(const Vertex *vertecies, unsigned int verteciesCount, const GLuint *indices, unsigned int indicesCount)
	{

		m_indicesCount = indicesCount;
		m_verteciesCount = verteciesCount;

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verteciesCount, vertecies, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position))); // Vertex
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal))); // Normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color))); // Color
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, texcoord))); // Texcoord

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR){
			std::cout << "OpenGL error on model initializing: " << error << std::endl;
			return false;
		}
		return true;

	}

	void Mesh::render(const glm::mat4x4 &pr_matrix, const glm::mat4x4 &vw_matrix, const glm::vec3 lightDir, const glm::vec3 viewDir){

		md_matrix = glm::mat4x4(1.0f);
		md_matrix = glm::translate(md_matrix, m_position);
		md_matrix = glm::rotate(md_matrix, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		md_matrix = glm::rotate(md_matrix, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		md_matrix = glm::rotate(md_matrix, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		md_matrix = glm::scale(md_matrix, m_size);

		m_material->use(pr_matrix, vw_matrix, md_matrix, lightDir, viewDir);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_material->release();

	}


} }