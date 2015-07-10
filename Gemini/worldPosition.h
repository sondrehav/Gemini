#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.h"
#include "vertex.h"
#include "worldPosition.h"

namespace gemini {
	namespace graphics{

		class WorldPosition
		{
		public:
			WorldPosition worldPosition;
		private:
			GLuint m_VAO, m_VBO, m_IBO;
			Shader *m_shader;
			glm::mat4x4 md_matrix;
			unsigned int m_indicesCount, m_verteciesCount;
		public:
			Mesh(Shader *shader);
			virtual ~Mesh();
			bool loadData(const Vertex *vertecies, unsigned int vertCount, const GLuint *indices, unsigned int indicesCount);
			void render(const glm::mat4x4 &pr_matrix, const glm::mat4x4 &vw_matrix);
			Mesh createInstance();
		};
	}
}