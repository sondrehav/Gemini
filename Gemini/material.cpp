#include "material.h"
#include <iostream>

namespace gemini {
	namespace graphics {

		Material::Material(std::string name, std::string vertexShader, std::string fragmentShader)
		{
			m_shader = new Shader(vertexShader, fragmentShader);
			m_name = name;
			m_shader->Bind();
			m_shader->SetUniform1i("diffuseMap", 0);
			m_shader->SetUniform1i("ambientMap", 1);
			m_shader->SetUniform1i("specularMap", 2);
			m_shader->SetUniform1i("specularHighlightMap", 3);
			m_shader->SetUniform1i("normalMap", 4);
			m_shader->SetUniform1i("alphaMap", 5);
			m_shader->SetUniform3f("diffuseColor", m_diffuseColor.r, m_diffuseColor.b, m_diffuseColor.g);
			m_shader->SetUniform3f("ambientColor", m_ambientColor.r, m_ambientColor.b, m_ambientColor.g);
			m_shader->SetUniform3f("specularColor", m_specularColor.r, m_specularColor.b, m_specularColor.g);
			m_shader->SetUniform1f("transparency", 1.0f);
			m_shader->Unbind();
		}

		void Material::use(const glm::mat4x4 &pr, const glm::mat4x4 &vw, const glm::mat4x4 &md)
		{
			m_shader->Bind();
			m_shader->SetUniformMat4("pr_matrix", pr);
			m_shader->SetUniformMat4("vw_matrix", vw);
			m_shader->SetUniformMat4("md_matrix", md);
			m_diffuse->bind(0);
			m_ambient->bind(1);
			m_specular->bind(2);
			m_specularHighlight->bind(3);
			m_normals->bind(4);
			m_alpha->bind(5);
		}

		void Material::release()
		{
			m_alpha->unbind(5);
			m_normals->unbind(4);
			m_specularHighlight->unbind(3);
			m_specular->unbind(2);	
			m_ambient->unbind(1);
			m_diffuse->unbind(0);
			m_shader->Unbind();
		}

		Material::~Material()
		{
			
		}

	}
}