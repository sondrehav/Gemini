#pragma once

#include <string>
#include "texture.h"
#include <glm\glm.hpp>
#include "shader.h"

namespace gemini {
	namespace graphics {
		class Material
		{
		public:
			Material(Shader *shader);
			void bind();
			void unbind();
			virtual ~Material();
		private:
			Shader* m_shader;
		public:

			std::string m_name;

			Texture* m_diffuseMap = NULL;
			Texture* m_ambientMap = NULL;
			Texture* m_specularColorMap = NULL;
			Texture* m_specularHighlightMap = NULL;
			Texture* m_normalMap = NULL;
			Texture* m_alphaMap = NULL;

			GLuint m_sampler_diffuse;
			GLuint m_sampler_ambient;
			GLuint m_sampler_specular;
			GLuint m_sampler_specularHighlight;
			GLuint m_sampler_normal;
			GLuint m_sampler_alpha;

			glm::vec3 m_ambientColor;
			glm::vec3 m_diffuseColor;
			glm::vec3 m_specularColor;
			float m_specularExp;
			float m_transparency;
			unsigned int m_illum;
		};
	}
}

