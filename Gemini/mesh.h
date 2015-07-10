#pragma once

#include <string>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>

#include "vertex.h"

namespace gemini {
	namespace graphics{

		class Mesh
		{
		private:
			struct MeshEntry {
				MeshEntry();
				~MeshEntry();
				bool init(const std::vector<Vertex> &vertecies, const std::vector<GLuint> &indices);
				GLuint m_VBO, m_IBO, m_VAO;
				unsigned int m_count;
				unsigned int m_materialIndex;
			};
			std::vector<MeshEntry> m_meshes;
			std::vector<int> m_textures; // TODO: Type
		public:
			Mesh();
			bool loadScene(std::string filepath);
			virtual ~Mesh();
			void render(Shader &shader);
		private:
			bool initFromScene(const aiScene* pScene, const std::string& filename);
			void initMesh(unsigned int index, const aiMesh* paiMesh);
			bool initMaterials(const aiScene* pScene, const std::string& Filename);
			void clear();
		};
	}
}