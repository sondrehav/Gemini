#include "shader.h"
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "textLoader.h"

static void CheckShaderError(GLuint shaderID, GLuint flag, bool isProgram, const char* errorMessage);
static GLuint CreateShader(const std::string &source, GLenum shaderType);

namespace gemini { namespace graphics {

	static std::vector<Shader*> allShaders;

	Shader::Shader(const std::string vertexShader, const std::string fragmentShader){

		m_program = glCreateProgram();

		std::cout << "Loading shader '" << vertexShader << "'." << std::endl;
		std::cout << "Loading shader '" << fragmentShader << "'." << std::endl;

		m_shaders[0] = CreateShader(util::LoadFile(vertexShader), GL_VERTEX_SHADER);
		m_shaders[1] = CreateShader(util::LoadFile(fragmentShader), GL_FRAGMENT_SHADER);

		for (int i = 0; i < NUM_SHADERS; i++){
			glAttachShader(m_program, m_shaders[i]);
		}

		glLinkProgram(m_program);
		CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: Program linking failed: ");

		glValidateProgram(m_program);
		CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: Program is invalid: ");

		GLint count;
		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &count);
		for (int i = 0; i < count; ++i) { // Caching of uniforms
			int nameLen = -1, num = -1;
			GLenum type = GL_ZERO;
			char name[100];
			glGetActiveUniform(m_program, GLuint(i), sizeof(name) - 1, &nameLen, &num, &type, name);
			name[nameLen] = 0;
			GLuint location = glGetUniformLocation(m_program, name);
			//std::cout << "UNIFORM: " << name << " at location " << location << std::endl;
			m_uniformLocations[std::string(name)] = location;
		}

		m_vertPath = vertexShader;
		m_fragPath = fragmentShader;

		allShaders.push_back(this);

	}

	void Shader::Bind(){
		glUseProgram(m_program);
	}

	void Shader::Unbind(){
		glUseProgram(0);
	}

	Shader::~Shader(){
		for (int i = 0; i < NUM_SHADERS; i++) {
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
		glDeleteProgram(m_program);
	}

	GLint Shader::getUniformLocation(std::string uniformName, bool suppressWarnings){
		std::unordered_map<std::string, GLuint>::const_iterator f = m_uniformLocations.find(uniformName);
		if (f == m_uniformLocations.end()){
			
		}
		else {
			return f->second;
		}
		
		GLint location = glGetUniformLocation(m_program, uniformName.c_str());
		if (location != -1){
			m_uniformLocations[uniformName] = location;
			return location;
		}
		if (!suppressWarnings)
			std::cerr << "ERROR: Uniform '" << uniformName << "' in shader does not exist!" << std::endl; // TODO: Output which shader!

		return -1;
	}

	void Shader::SetUniform1f(std::string name, float value){ glUniform1f(getUniformLocation(name), value); }
	void Shader::SetUniform2f(std::string name, float x, float y){ glUniform2f(getUniformLocation(name), x, y); }
	void Shader::SetUniform3f(std::string name, float x, float y, float z){ glUniform3f(getUniformLocation(name), x, y, z); }
	void Shader::SetUniform4f(std::string name, float x, float y, float z, float w){ glUniform4f(getUniformLocation(name), x, y, z, w); }
	void Shader::SetUniform1i(std::string name, int value){ glUniform1i(getUniformLocation(name), value); }
	void Shader::SetUniformMat4(std::string name, const glm::mat4x4 &matrix){ glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, (GLfloat*)&matrix); }
	bool Shader::hasUniformLocation(std::string name)
	{
		std::unordered_map<std::string, GLuint>::iterator it = m_uniformLocations.find(name);
		GLuint b3;
		if (it != m_uniformLocations.end())
		{
			return true;
		}
		return false;
	}

	void Shader::upload1fToAll(std::string uniform, GLfloat value)
	{
		for (int i = 0; i < allShaders.size(); i++)
		{
			Shader* s = allShaders[i];
			s->Bind();
			if (s->hasUniformLocation(uniform))
			{
				s->SetUniform1f(uniform, value);
			}
		}
	}

	void Shader::upload3fToAll(std::string uniform, float a, float b, float c)
	{
		for (int i = 0; i < allShaders.size(); i++)
		{
			Shader* s = allShaders[i];
			s->Bind();
			if (s->hasUniformLocation(uniform))
			{
				s->SetUniform3f(uniform, a, b, c);
			}
		}
	}

} }

static GLuint CreateShader(const std::string &source, GLenum shaderType){

	GLint shaderid = glCreateShader(shaderType);
	if (shaderid == 0) std::cerr << "ERROR: Shader creation failed!" << std::endl;
	const GLchar* shaderSource[1];
	GLint shaderSourceLength[1];
	shaderSource[0] = source.c_str();
	shaderSourceLength[0] = source.length();
	glShaderSource(shaderid, 1, shaderSource, shaderSourceLength);
	glCompileShader(shaderid);
	CheckShaderError(shaderid, GL_COMPILE_STATUS, false, "ERROR: Shader failed to compile: ");
	return shaderid;

}


static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const char* errorMessage){
	GLint success = 0;
	GLchar errorlog[1024] = { 0 };
	if (isProgram) glGetProgramiv(shader, flag, &success);
	else glGetShaderiv(shader, flag, &success);
	if (success == GL_FALSE) {
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(errorlog), NULL, errorlog);
		else
			glGetShaderInfoLog(shader, sizeof(errorlog), NULL, errorlog);
		std::cerr << errorMessage << "'" << errorlog << "'" << std::endl;
		system("pause");
	}
}