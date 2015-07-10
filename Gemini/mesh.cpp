#include <glm/gtc/matrix_transform.hpp>
#include "mesh.h"
#include <iostream>
#include <assimp\postprocess.h>
#include "vertex.h"

namespace gemini { namespace graphics{

	Mesh::Mesh()
	{
		
	}

	Mesh::~Mesh()
	{

	}

	bool Mesh::loadScene(std::string filepath)
	{
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(filepath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
		bool success;
		if (pScene) {
			std::cout << "Model '" << filepath << "' loaded." << std::endl;
			success = true;
		}
		else {
			std::cerr << "Error parsing model '" << filepath << "'." << std::endl;
			success = false;
		}
		return success;
	}

	bool Mesh::initFromScene(const aiScene* pScene, const std::string& filename)
	{
		m_meshes.resize(pScene->mNumMeshes);
		m_textures.resize(pScene->mNumMaterials);
		for (int i = 0; i < m_meshes.size(); i++){
			const aiMesh* paiMesh = pScene->mMeshes[i];
			initMesh(i, paiMesh);
		}
		return initMaterials(pScene, filename);
	}

	void Mesh::initMesh(unsigned int index, const aiMesh* paiMesh)
	{
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
		m_meshes[index].m_materialIndex = paiMesh->mMaterialIndex;
		std::vector<Vertex> vertecies;
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
			const aiVector3D* pos = &(paiMesh->mVertices[i]);
			const aiVector3D* norm = &(paiMesh->mNormals[i]);
			const aiVector3D* texcoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
			Vertex v(glm::vec3(pos->x, pos->y, pos->z), glm::vec3(norm->x, norm->y, norm->z), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(texcoord->x, texcoord->y));
			vertecies.push_back(v);
		}
		std::vector<GLuint> indices;
		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++){
			const aiFace& face = paiMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
		m_meshes[index].init(vertecies, indices);
	}
	
	bool Mesh::initMaterials(const aiScene* pScene, const std::string& Filename)
	{
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
			const aiMaterial* pMaterials = pScene->mMaterials[i];
		}
		return true;
	}

	void Mesh::clear()
	{

	}

	Mesh::MeshEntry::MeshEntry(){}
	Mesh::MeshEntry::~MeshEntry(){}

	bool Mesh::MeshEntry::init(const std::vector<Vertex> &vertecies, const std::vector<GLuint> &indices)
	{

		glGenVertexArrays(1, &this->m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &this->m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertecies.size(), &vertecies, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));

		glGenBuffers(1, &this->m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		return true;

	}

	void Mesh::render(Shader &shader){

		shader.Bind();
		glm::mat4x4 pr_matrix = glm::perspective(60.0f, 1200.0f / 800.0f, 0.01f, 1000.0f);
		glm::mat4x4 vw_matrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f,0.0f,-100.0f));
		shader.SetUniformMat4("pr_matrix", pr_matrix);
		shader.SetUniformMat4("vw_matrix", vw_matrix);
		for (unsigned int i = 0; i < m_meshes.size(); i++){
			MeshEntry mesh = m_meshes[i];
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_IBO);
			glDrawElements(GL_TRIANGLES, mesh.m_count, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		shader.Unbind();

	}

} }