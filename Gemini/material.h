#pragma once

#include <string>
#include "texture.h"
#include <glm\glm.hpp>
#include "shader.h"
#include <unordered_map>

#define DEF_VERTEX_SHADER "res/default.vs"
#define DEF_FRAGMENT_SHADER "res/default.fs"

namespace gemini {
	namespace graphics {

		class Material
		{
		public:
			
			Material(std::string name)
			{
				Material(name, DEF_VERTEX_SHADER, DEF_FRAGMENT_SHADER);
			}
			Material(std::string name, std::string vertexShader, std::string fragmentShader);
			virtual ~Material();

			inline void setDiffuseTexture(Texture* diffuse) {m_diffuse = diffuse; }
			inline void setNormalTexture(Texture* normal) { m_normals = normal; }
			inline void setSpecularTexture(Texture* specular) { m_specular = specular; }
			inline void setSpecHighlightTexture(Texture* highlight) { m_specularHighlight = highlight; }
			inline void setAmbientTexture(Texture* ambient) { m_ambient = ambient; }
			inline void setAlphaTexture(Texture* alpha) { m_alpha = alpha; }

			inline void setDiffuseColor(float r, float g, float b)
			{
				m_diffuseColor.r = r; m_diffuseColor.g = g; m_diffuseColor.b = b;
				m_shader->Bind();
				m_shader->SetUniform3f("diffuseColor", m_diffuseColor.r, m_diffuseColor.b, m_diffuseColor.g);
				m_shader->Unbind();
			}
			inline void setAmbientColor(float r, float g, float b)
			{
				m_ambientColor.r = r; m_ambientColor.g = g; m_ambientColor.b = b;
				m_shader->Bind();
				m_shader->SetUniform3f("ambientColor", m_ambientColor.r, m_ambientColor.b, m_ambientColor.g);
				m_shader->Unbind();
			}
			inline void setSpecularColor(float r, float g, float b)
			{
				m_specularColor.r = r; m_specularColor.g = g; m_specularColor.b = b;
				m_shader->Bind();
				m_shader->SetUniform3f("specularColor", m_specularColor.r, m_specularColor.b, m_specularColor.g);
				m_shader->Unbind();
			}
			inline void setTransparency(float f) {
				m_transparency = f;
				m_shader->Bind();
				m_shader->SetUniform1f("transparency", f);
				m_shader->Unbind();
			}

			void use(const glm::mat4x4 &pr, const glm::mat4x4 &vw, const glm::mat4x4 &md, const glm::vec3 front, const glm::vec3 lightDir);
			void release();

		private:

			std::string m_name;

			Texture* m_diffuse = Texture::addTexture(0xFF, 0xFF, 0xFF, 0xFF);
			Texture* m_normals = Texture::addTexture(0x7F, 0x7F, 0xFF, 0xFF);
			Texture* m_specular = Texture::addTexture(0x00, 0x00, 0x00, 0xFF);
			Texture* m_specularHighlight = Texture::addTexture(0x00, 0x00, 0x00, 0xFF);
			Texture* m_ambient = Texture::addTexture(0x00, 0x00, 0x00, 0xFF);
			Texture* m_alpha = Texture::addTexture(0xFF, 0xFF, 0xFF, 0xFF);

			glm::vec3 m_diffuseColor = glm::vec3(1.0f);
			glm::vec3 m_ambientColor = glm::vec3(0.0f);
			glm::vec3 m_specularColor = glm::vec3(0.0f);

			float m_transparency = 1.0f;

			Shader* m_shader;

		};
	}
}

