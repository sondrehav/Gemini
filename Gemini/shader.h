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
			GLint Shader::getUniformLocation(std::string name, bool showWarnings = true);
			bool Shader::hasUniformLocation(std::string name);

			inline std::string vertPath(){ return m_vertPath; }
			inline std::string fragPath(){ return m_fragPath; }

			static void upload1fToAll(std::string uniform, GLfloat value);
			static void upload3fToAll(std::string uniform, float a, float b, float c);

		private:
			std::string m_vertPath;
			std::string m_fragPath;
			static const unsigned int NUM_SHADERS = 2;
			GLuint m_program;
			GLuint m_shaders[NUM_SHADERS];
			std::unordered_map<std::string, GLuint> m_uniformLocations;
		};

	}
}