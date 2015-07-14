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
			static Texture* addTexture(const std::string filename);
			static Texture* addTexture(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
			void bind(GLuint active = 0);
			void unbind(GLuint active = 0);
			inline std::string getPath() { return m_name; }
			virtual ~Texture();
		private:
			Texture(const std::string filename);
			Texture(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
			GLuint m_textureid = -1;
			GLenum m_target;
			std::string m_name;
		};
	}
}