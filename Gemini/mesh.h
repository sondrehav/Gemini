#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.h"
#include "vertex.h"
#include "texture.h"
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
			Shader *m_shader;
			glm::mat4x4 md_matrix;
			unsigned int m_indicesCount, m_verteciesCount;
			Texture *m_texture = NULL;
		public:
			Mesh(Shader *shader);
			virtual ~Mesh();
			bool loadData(const Vertex *vertecies, unsigned int vertCount, const GLuint *indices, unsigned int indicesCount, Texture *texture);
			bool loadMesh(const std::string &filename);
			//void setTexture(Texture *texture);
			Texture* getTexture();
			void render(const glm::mat4x4 &pr_matrix, const glm::mat4x4 &vw_matrix);
			Mesh createInstance();
		};
	}
}