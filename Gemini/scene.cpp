#include "scene.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include "vertex.h"
#include <glm\glm.hpp>
#include <assert.h>
#include <assimp/Exporter.hpp>
#include "material.h"

namespace gemini {
	namespace graphics {

		Scene::Scene(Shader *shader)
		{
			m_shader = shader;
		}

		Scene::~Scene()
		{

		}

		void Scene::render(const glm::mat4x4 &pr_matrix, const glm::mat4x4 &vw_matrix)
		{
			for (unsigned int i = 0; i < m_meshes.size(); i++)
			{
				m_meshes[i]->render(pr_matrix, vw_matrix);
			}
		}

		bool Scene::loadScene(const std::string &filename, Shader* shader)
		{

			std::string directory;
			const size_t last_slash_idx = filename.rfind('/');
			if (std::string::npos != last_slash_idx)
			{
				directory = filename.substr(0, last_slash_idx);
			}
			
			Assimp::Importer importer;
			const aiScene* p_scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_SortByPType);
			
			if (!p_scene)
			{
				std::cerr << "Could not load '" << filename << "'." << std::endl; 
				std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
				return false;
			}

			m_meshes.resize(p_scene->mNumMeshes);
			m_materials.resize(p_scene->mNumMaterials);


			for (unsigned int i = 0; i < m_materials.size(); i++)
			{
				const aiMaterial* p_material = p_scene->mMaterials[i];
				Material* m = new Material(shader);
				m_materials[i] = m;
				aiString mname;
				p_material->Get(AI_MATKEY_NAME, mname);
				if (mname.length > 0)
					m->m_name = mname.C_Str();
				std::cout << "Loading material '" << m->m_name << "'." << std::endl;
				int shadingModel;
				p_material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
				int phong = aiShadingMode_Phong;
				if (p_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{
					aiString path;
					if (p_material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
					{
						std::string texturePath = directory + "/" + path.data;
						size_t finder;
						while ((finder = texturePath.rfind("\\")) != std::string::npos)
						{
							texturePath.replace(finder, std::string("\\").length(), "/");
						}
						Texture* t = new Texture();
						t->load(texturePath);
						m_materials[i]->m_diffuseMap = t;
					}
				}
				if (p_material->GetTextureCount(aiTextureType_HEIGHT) > 0)
				{
					aiString path;
					if (p_material->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
					{
						std::string texturePath = directory + "/" + path.data;
						size_t finder;
						while ((finder = texturePath.rfind("\\")) != std::string::npos)
						{
							texturePath.replace(finder, std::string("\\").length(), "/");
						}
						Texture* t = new Texture();
						t->load(texturePath);
						m_materials[i]->m_normalMap = t;
					}
				}
				if (p_material->GetTextureCount(aiTextureType_SPECULAR) > 0)
				{
					aiString path;
					if (p_material->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
					{
						std::string texturePath = directory + "/" + path.data;
						size_t finder;
						while ((finder = texturePath.rfind("\\")) != std::string::npos)
						{
							texturePath.replace(finder, std::string("\\").length(), "/");
						}
						Texture* t = new Texture();
						t->load(texturePath);
						m_materials[i]->m_specularColorMap = t;
					}
				}
				if (p_material->GetTextureCount(aiTextureType_SHININESS) > 0)
				{
					aiString path;
					if (p_material->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
					{
						std::string texturePath = directory + "/" + path.data;
						size_t finder;
						while ((finder = texturePath.rfind("\\")) != std::string::npos)
						{
							texturePath.replace(finder, std::string("\\").length(), "/");
						}
						Texture* t = new Texture();
						t->load(texturePath);
						m_materials[i]->m_specularHighlightMap = t;
					}
				}
				if (p_material->GetTextureCount(aiTextureType_AMBIENT) > 0)
				{
					aiString path;
					if (p_material->GetTexture(aiTextureType_AMBIENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
					{
						std::string texturePath = directory + "/" + path.data;
						size_t finder;
						while ((finder = texturePath.rfind("\\")) != std::string::npos)
						{
							texturePath.replace(finder, std::string("\\").length(), "/");
						}
						Texture* t = new Texture();
						t->load(texturePath);
						m_materials[i]->m_ambientMap = t;
					}
				}
				if (p_material->GetTextureCount(aiTextureType_OPACITY) > 0)
				{
					aiString path;
					if (p_material->GetTexture(aiTextureType_OPACITY, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
					{
						std::string texturePath = directory + "/" + path.data;
						size_t finder;
						while ((finder = texturePath.rfind("\\")) != std::string::npos)
						{
							texturePath.replace(finder, std::string("\\").length(), "/");
						}
						Texture* t = new Texture();
						t->load(texturePath);
						m_materials[i]->m_alphaMap = t;
					}
					/*else {
						std::cerr << "Could not load '" << path.data << "' in scene loader." << std::endl;
						return false;
					}*/
				}
			}

			for (unsigned int i = 0; i < m_meshes.size(); i++)
			{
				aiMesh* paiMesh = p_scene->mMeshes[i];
				m_meshes[i] = initMesh(paiMesh, m_shader);
				std::cout << "Loaded mesh '" << &(paiMesh->mName) << "'." << std::endl;
				unsigned int vcount = p_scene->mMeshes[i]->mNumVertices;
				unsigned int fcount = p_scene->mMeshes[i]->mNumFaces;
			}

			std::cout << "Loaded scene '" << filename << "'." << std::endl;

			return true;

		}

		Mesh* Scene::initMesh(aiMesh* paiMesh, Shader *shader){

			std::vector<Vertex> vertecies;
			unsigned int v_count = paiMesh->mNumVertices;
			vertecies.resize(v_count);

			std::vector<GLuint> faces;
			unsigned int i_count = paiMesh->mNumFaces;
			faces.resize(i_count * 3);

			const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);
			const aiColor4D zeroCol(0.0f, 0.0f, 0.0f, 0.0f);

			const float fefe = 1.0f;
			for (unsigned int i = 0; i < v_count; i++)
			{

				const aiVector3D* pos = &(paiMesh->mVertices[i]);
				const aiVector3D* normal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &zeroVec;
				const aiColor4D* color = paiMesh->HasVertexColors(0) ? paiMesh->mColors[i] : &zeroCol;
				const aiVector3D* texCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zeroVec;

				Vertex v(
					glm::vec3(pos->x, pos->y, pos->z),
					glm::vec3(normal->x, normal->y, normal->z),
					glm::vec3(color->r, color->b, color->g),
					glm::vec2(texCoord->x, texCoord->y)
				);
				if (texCoord->x>1.0f || texCoord->x<0.0f || texCoord->y>1.0f || texCoord->y < 0.0f){
					if (false){}
				}

				vertecies[i] = v;

			}

			for (unsigned int i = 0; i < i_count; i++)
			{

				const aiFace* face = &(paiMesh->mFaces[i]);
				assert(face->mNumIndices == 3);
				faces[i * 3 + 0] = face->mIndices[0];
				faces[i * 3 + 1] = face->mIndices[1];
				faces[i * 3 + 2] = face->mIndices[2];

			}

			Mesh* m = new Mesh(shader);
			m->loadData(&vertecies[0], v_count, &faces[0], i_count * 3, m_materials[paiMesh->mMaterialIndex]);

			return m;

		}

	}
}

