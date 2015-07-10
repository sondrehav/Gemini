#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <glm/glm.hpp>

namespace gemini {
	namespace graphics {

		class Shader
		{
		public:
			Shader(const std::string vertexShader, const std::string fragmentShader);
			void Bind();
			void Unbind();
			virtual ~Shader();

			void SetUniform1f(std::string uniformName, float a);
			void SetUniform2f(std::string uniformName, float a, float b);
			void SetUniform3f(std::string uniformName, float a, float b, float c);
			void SetUniform4f(std::string uniformName, float a, float b, float c, float d);
			void SetUniform1i(std::string uniformName, int a);
			void SetUniformMat4(std::string uniformName, const glm::mat4x4 &matrix);

		private:
			GLint Shader::getUniformLocation(std::string name);
			static const unsigned int NUM_SHADERS = 2;
			GLuint m_program;
			GLuint m_shaders[NUM_SHADERS];
			std::unordered_map<std::string, GLuint> m_uniformLocations;
		};

	}
}