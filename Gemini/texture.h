#pragma once

#include <string>
#include <GL\glew.h>
#include "SOIL/SOIL.h"
#include <unordered_map>

namespace gemini {
	namespace graphics {
		class Texture
		{
		public:
			Texture();
			Texture(const std::string &filename);
			bool load(const std::string &filename);
			void bind();
			void unbind();
			virtual ~Texture();
			static Texture* getDefault();
		private:
			GLuint m_textureid = -1;
			GLenum m_target;
		};
	}
}