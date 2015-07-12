#pragma once
#include <string>
#include <vector>
#include "mesh.h"
#include "texture.h"
#include <assimp/scene.h>
#include "shader.h"

namespace gemini {
	namespace graphics {
		class Scene
		{
		public:
			Scene(Shader *shader);
			bool loadScene(const std::string &filename);
			virtual ~Scene();
			void render(const glm::mat4x4 &pr_matrix, const glm::mat4x4 &vw_matrix);
		private:
			Mesh* initMesh(aiMesh* paiMesh, Shader *shader);
		private:
			Shader *m_shader;
			std::vector<Mesh*> m_meshes;
			std::vector<Texture*> m_materials;
		};
	}
}

