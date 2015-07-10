#include "texture.h"

namespace gemini {
	namespace graphics {

		Texture::Texture()
		{
		}


		Texture::~Texture()
		{
		}

		bool Texture::load(const std::string &filename, GLenum textureTarget)
		{
			return false;
		}

		void Texture::bind()
		{
			glBindTexture(m_target, m_textureid);
		}

		void Texture::unbind()
		{
			glBindTexture(m_target, 0);
		}

	}
}

