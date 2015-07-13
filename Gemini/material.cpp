#include "material.h"


namespace gemini {
	namespace graphics {

		Material::Material(Shader *shader)
		{
			m_shader = shader;

			m_shader->Bind();

			/*
			
			diffuseMap;
			ambientMap;
			specularMap;
			specularHighlightMap;
			normalMap;
			alphaMap;
			
			*/

			shader->SetUniform1i("diffuseMap", 0);
			shader->SetUniform1i("ambientMap", 1);
			shader->SetUniform1i("specularMap", 2);
			shader->SetUniform1i("specularHighlightMap", 3);
			shader->SetUniform1i("normalMap", 4);
			shader->SetUniform1i("alphaMap", 5);

			m_shader->Unbind();

		}

		void Material::bind()
		{
			if (m_diffuseMap != NULL)
			{
				glActiveTexture(GL_TEXTURE0);
				m_diffuseMap->bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				Texture::getDefault()->bind();
			}
			if (m_ambientMap != NULL)
			{
				glActiveTexture(GL_TEXTURE1);
				m_ambientMap->bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE1);
				Texture::getDefault()->bind();
			}
			if (m_specularColorMap != NULL)
			{
				glActiveTexture(GL_TEXTURE2);
				m_specularColorMap->bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE2);
				Texture::getDefault()->bind();
			}
			if (m_specularHighlightMap != NULL)
			{
				glActiveTexture(GL_TEXTURE3);
				m_specularHighlightMap->bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE3);
				Texture::getDefault()->bind();
			}
			if (m_normalMap != NULL)
			{
				glActiveTexture(GL_TEXTURE4);
				m_normalMap->bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE4);
				Texture::getDefault()->bind();
			}
			if (m_alphaMap != NULL)
			{
				glActiveTexture(GL_TEXTURE5);
				m_alphaMap->bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE5);
				Texture::getDefault()->bind();
			}
		}

		void Material::unbind()
		{
			if (m_diffuseMap != NULL)
			{
				glActiveTexture(GL_TEXTURE0);
				m_diffuseMap->unbind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				Texture::getDefault()->unbind();
			}
			if (m_ambientMap != NULL)
			{
				glActiveTexture(GL_TEXTURE1);
				m_ambientMap->unbind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE1);
				Texture::getDefault()->unbind();
			}
			if (m_specularColorMap != NULL)
			{
				glActiveTexture(GL_TEXTURE2);
				m_specularColorMap->unbind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE2);
				Texture::getDefault()->unbind();
			}
			if (m_specularHighlightMap != NULL)
			{
				glActiveTexture(GL_TEXTURE3);
				m_specularHighlightMap->unbind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE3);
				Texture::getDefault()->unbind();
			}
			if (m_normalMap != NULL)
			{
				glActiveTexture(GL_TEXTURE4);
				m_normalMap->unbind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE4);
				Texture::getDefault()->unbind();
			}
			if (m_alphaMap != NULL)
			{
				glActiveTexture(GL_TEXTURE5);
				m_alphaMap->unbind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE5);
				Texture::getDefault()->unbind();
			}
		}

		Material::~Material()
		{
			delete m_diffuseMap;
			delete m_ambientMap;
			delete m_specularColorMap;
			delete m_specularHighlightMap;
			delete m_normalMap;
			delete m_alphaMap;
		}

	}
}