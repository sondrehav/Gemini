#pragma once

#include <string>
#include <GL\glew.h>
#include "SOIL/SOIL.h"

namespace gemini {
	namespace graphics {
		class Texture
		{
		public:
			Texture();
			bool load(const std::string &filename);
			void bind();
			void unbind();
			virtual ~Texture();
		private:
			GLuint m_textureid;
			GLenum m_target;
		};	
	}
}