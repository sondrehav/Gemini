#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.h"
#include "vertex.h"
#include "material.h"
#include <string>

namespace gemini {
	namespace graphics{

		class Mesh
		{
		public:
			glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 m_size = glm::vec3(1.0f, 1.0f, 1.0f);
		private:
			GLuint m_VAO, m_VBO, m_IBO;
			glm::mat4x4 md_matrix;
			unsigned int m_indicesCount, m_verteciesCount;
			Material *m_material;
		public:
			Mesh(Material* material, const Vertex *vertecies, unsigned int vertCount, const GLuint *indices, unsigned int indicesCount);
			virtual ~Mesh();
			inline Material* getTexture() { return m_material; }
			void render(const glm::mat4x4 &pr_matrix, const glm::mat4x4 &vw_matrix);
		private:
			bool setData(const Vertex *vertecies, unsigned int verteciesCount, const GLuint *indices, unsigned int indicesCount);
		};
	}
}