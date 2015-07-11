#include "texture.h"
#include <iostream>

namespace gemini {
	namespace graphics {

		Texture::Texture()
		{
		}


		Texture::~Texture()
		{
		}

		bool Texture::load(const std::string &filename)
		{

			m_textureid = SOIL_load_OGL_texture(
				filename.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);
		
			GLenum error = glGetError();
			if (error != GL_NO_ERROR){
				std::cout << "OpenGL error on texture initializing: '"<<filename<<"'. Errorcode: " << error << std::endl;
				return false;
			}

			if (m_textureid == 0){
				std::cout << "Error on texture initializing: '" << filename << "'. ID recieved was 0." << std::endl;
				return false;
			}

			return true;
		}

		void Texture::bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_textureid);
		}

		void Texture::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}

