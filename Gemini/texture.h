#pragma once

#include <string>
#include <GL\glew.h>

namespace gemini {
	namespace graphics {
		class Texture
		{
		public:
			Texture();
			bool load(const std::string &filename, GLenum textureTarget = GL_TEXTURE_2D);
			void bind();
			void unbind();
			virtual ~Texture();
		private:
			GLuint m_textureid;
			GLenum m_target;
		};	
	}
}