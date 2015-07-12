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
			virtual ~Material();
		private:
			Shader* m_shader;
			Texture* m_diffuseMap;
			Texture* m_ambientMap;
			Texture* m_specularColorMap;
			Texture* m_specularHighlightMap;
			Texture* m_normalMap;
			Texture* m_alphaMap;
			glm::vec3 m_ambientColor;
			glm::vec3 m_diffuseColor;
			glm::vec3 m_specularColor;
			float m_specularExp;
			float m_transparency;
			unsigned int m_illum;
		};
	}
}

