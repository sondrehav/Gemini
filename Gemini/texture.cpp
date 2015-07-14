#include "texture.h"
#include <iostream>
#include <unordered_map>

namespace gemini {
	namespace graphics {

		static std::unordered_map<std::string, Texture*> textureCache;
		static std::unordered_map<unsigned int, Texture*> colorTextureCache;

		Texture* Texture::addTexture(const std::string filename)
		{
			std::unordered_map<std::string, Texture*>::const_iterator f = textureCache.find(filename);
			if (f == textureCache.end()){
				Texture* t = new Texture(filename);
				textureCache[filename] = t;
				return t;
			}
			else {
				return f->second;
			}
		}

		Texture::Texture(const std::string filename)
		{
			m_textureid = SOIL_load_OGL_texture(
				filename.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS
			);
		
			GLenum error = glGetError();
			if (error != GL_NO_ERROR){
				std::cerr << "OpenGL error on texture initializing: '" << filename << "'. Errorcode: " << error << std::endl; return;
			}

			if (m_textureid == 0){
				std::cerr << "Error on texture initializing: '" << filename << "'. ID recieved was 0." << std::endl;
				return;
			}


			m_name = filename;

			std::cout << "Successfully loaded '" << filename << "'." << std::endl;
		}

		Texture* Texture::addTexture(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			unsigned int colId = r << 24 | g << 16 | b << 8 | a << 0;
			std::unordered_map<unsigned int, Texture*>::const_iterator f = colorTextureCache.find(colId);
			if (f == colorTextureCache.end()){
				Texture* texture = new Texture(r, g, b, a);
				colorTextureCache[colId] = texture;
				return texture;
			}
			else {
				return f->second;
			}
		}

		Texture::Texture(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			glGenTextures(1, &m_textureid);

			if (m_textureid == 0){
				std::cerr << "Error on texture initializing color texture. ID recieved was 0." << std::endl;
				return;
			}

			unsigned char data[]{r, g, b, a};
			glBindTexture(GL_TEXTURE_2D, m_textureid);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR){
				std::cerr << "OpenGL error on texture initializing color texture. Errorcode: " << error << std::endl;
				return;
			}

			m_name = "COLOR_MAP";

		}


		Texture::~Texture()
		{

		}

		void Texture::bind(GLuint active)
		{
			glActiveTexture(GL_TEXTURE0 + active);
 			glBindTexture(GL_TEXTURE_2D, m_textureid);
		}

		void Texture::unbind(GLuint active)
		{
			glActiveTexture(GL_TEXTURE0 + active);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
	}
}

