#include "scene.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include "vertex.h"
#include <glm\glm.hpp>
#include <assert.h>
#include <assimp/Exporter.hpp>
#include "material.h"
#include "stringFunctions.h"

namespace gemini {
	namespace graphics {

		Scene::Scene()
		{}

		Scene::~Scene()
		{}

		void Scene::render(const glm::mat4x4 &pr_matrix, const glm::mat4x4 &vw_matrix)
		{
			for (unsigned int i = 0; i < m_meshes.size(); i++)
			{
				m_meshes[i]->render(pr_matrix, vw_matrix);
			}
		}

		bool Scene::loadScene(const std::string &filename)
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
				aiString mname;
				p_material->Get(AI_MATKEY_NAME, mname);
				std::string materialName;
				if (mname.length > 0)
					materialName = mname.C_Str();
				Material* m;
				if (materialName == "DefaultMaterial")
				{
					m = new Material(materialName, "res/default.vs", "res/default.fs");
					m_materials[i] = m;
					continue;
				}
				else
				{
					m = new Material(materialName, "res/material.vs", "res/material.fs");
				}
				std::cout << "Loading material '" << materialName << "'." << std::endl;
				m_materials[i] = m;

				aiString path;

				// Diffuse texture

				if (p_material->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
					p_material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string texturePath = directory + "/" + helper::replace(path.data, "\\", "/");
					m->setDiffuseTexture(Texture::addTexture(texturePath));
				}

				// Normal texture

				if (p_material->GetTextureCount(aiTextureType_HEIGHT) > 0 &&
					p_material->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string texturePath = directory + "/" + helper::replace(path.data, "\\", "/");
					m->setNormalTexture(Texture::addTexture(texturePath));
				}

				// Specular texture

				if (p_material->GetTextureCount(aiTextureType_SPECULAR) > 0 &&
					p_material->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string texturePath = directory + "/" + helper::replace(path.data, "\\", "/");
					m->setSpecularTexture(Texture::addTexture(texturePath));
				}

				// SpecularH texture

				if (p_material->GetTextureCount(aiTextureType_SHININESS) > 0 &&
					p_material->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string texturePath = directory + "/" + helper::replace(path.data, "\\", "/");
					m->setSpecHighlightTexture(Texture::addTexture(texturePath));
				}

				// Ambient texture

				if (p_material->GetTextureCount(aiTextureType_AMBIENT) > 0 &&
					p_material->GetTexture(aiTextureType_AMBIENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string texturePath = directory + "/" + helper::replace(path.data, "\\", "/");
					m->setAmbientTexture(Texture::addTexture(texturePath));
				}

				// Alpha texture

				if (p_material->GetTextureCount(aiTextureType_OPACITY) > 0 &&
					p_material->GetTexture(aiTextureType_OPACITY, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string texturePath = directory + "/" + helper::replace(path.data, "\\", "/");
					m->setAlphaTexture(Texture::addTexture(texturePath));
				}

				// Colors

				aiColor3D dif(1.0f, 1.0f, 1.0f);
				aiColor3D amb(0.0f, 0.0f, 0.0f);
				aiColor3D spec(0.0f, 0.0f, 0.0f);
				p_material->Get(AI_MATKEY_COLOR_DIFFUSE, dif);
				p_material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
				p_material->Get(AI_MATKEY_COLOR_SPECULAR, spec);

				m->setDiffuseColor(dif.r, dif.b, dif.g);
				m->setAmbientColor(amb.r, amb.b, amb.g);
				m->setSpecularColor(spec.r, spec.b, spec.g);

				// Transparency

				float trans = 1.0f;
				float transparency = p_material->Get(AI_MATKEY_OPACITY, trans);
				m->setTransparency(trans);
			}

			for (unsigned int i = 0; i < m_meshes.size(); i++)
			{
				aiMesh* paiMesh = p_scene->mMeshes[i];
				m_meshes[i] = initMesh(paiMesh);
				std::cout << "Loaded mesh '" << &(paiMesh->mName) << "'." << std::endl;
				unsigned int vcount = p_scene->mMeshes[i]->mNumVertices;
				unsigned int fcount = p_scene->mMeshes[i]->mNumFaces;
			}

			std::cout << "Loaded scene '" << filename << "'." << std::endl;

			return true;

		}

		Mesh* Scene::initMesh(aiMesh* paiMesh){

			std::vector<Vertex> vertecies;
			unsigned int v_count = paiMesh->mNumVertices;
			vertecies.resize(v_count);

			std::vector<GLuint> faces;
			unsigned int i_count = paiMesh->mNumFaces;
			faces.resize(i_count * 3);

			const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);
			const aiColor4D zeroCol(0.0f, 0.0f, 0.0f, 0.0f);

			for (unsigned int i = 0; i < v_count; i++)
			{

				const aiVector3D* pos = &(paiMesh->mVertices[i]);
				const aiVector3D* normal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &zeroVec;
				const aiVector3D* texCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zeroVec;

				glm::vec3 norm(normal->x, normal->y, normal->z);
				norm = glm::normalize(norm);

				Vertex v(
					glm::vec3(pos->x, pos->y, pos->z),
					norm,
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec2(texCoord->x, texCoord->y)
				);

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

			glm::vec3* tan1 = new glm::vec3[vertecies.size()];
			glm::vec3* tan2 = new glm::vec3[vertecies.size()];

			for (unsigned int i = 0; i < faces.size(); i+=3)
			{

				unsigned int i1 = faces[i + 0];
				unsigned int i2 = faces[i + 1];
				unsigned int i3 = faces[i + 2];

				glm::vec3 v1 = vertecies[i1].position;
				glm::vec3 v2 = vertecies[i2].position;
				glm::vec3 v3 = vertecies[i3].position;

				glm::vec2 w1 = vertecies[i1].texcoord;
				glm::vec2 w2 = vertecies[i2].texcoord;
				glm::vec2 w3 = vertecies[i3].texcoord;

				float x1 = v2.x - v1.x;
				float x2 = v3.x - v1.x;
				float y1 = v2.y - v1.y;
				float y2 = v3.y - v1.y;
				float z1 = v2.z - v1.z;
				float z2 = v3.z - v1.z;

				float s1 = w2.x - w1.x;
				float s2 = w3.x - w1.x;
				float t1 = w2.y - w1.y;
				float t2 = w3.y - w1.y;

				float r = 1.0f / (s1 * t2 - s2 * t1);

				glm::vec3 sdir = glm::vec3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
				glm::vec3 tdir = glm::vec3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

				tan1[i1] += sdir;
				tan1[i2] += sdir;
				tan1[i3] += sdir;

				tan2[i1] += tdir;
				tan2[i2] += tdir;
				tan2[i3] += tdir;

			}

			for (long a = 0; a < vertecies.size(); a++)
			{
				glm::vec3 n = vertecies[a].normal;
				glm::vec3 t = tan1[a];

				glm::vec3 v = glm::normalize((t - n * glm::dot(n, t)));

				if (glm::dot(glm::cross(n, t), tan2[a]) < 0.0f)
				{
					v *= -1;
				}

				vertecies[a].tangent = v;
			}

			Mesh* m = new Mesh(m_materials[paiMesh->mMaterialIndex], & vertecies[0], v_count, &faces[0], i_count * 3);

			delete tan1;
			delete tan2;

			return m;

		}

	}
}

