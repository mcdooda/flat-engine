#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include "program.h"

namespace flat
{
namespace video
{

Program::Program() :
	m_programId(0),
	m_valid(false)
{
	
}

Program::~Program()
{
	if (glIsProgram(m_programId))
		glDeleteProgram(m_programId);
}

void Program::load(std::string fragmentShader, std::string vertexShader)
{
	m_fragmentShader = fragmentShader;
	m_vertexShader = vertexShader;
	m_valid = true;
	
	GLuint fragmentShaderId = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
	checkShader(fragmentShader, fragmentShaderId);
	
	GLuint vertexShaderId = compileShader(vertexShader, GL_VERTEX_SHADER);
	checkShader(vertexShader, vertexShaderId);
	
	m_programId = compileProgram(fragmentShaderId, vertexShaderId);
	checkProgram(m_programId);
	
	loadAttributes();
	loadUniforms();
}

void Program::use(Window* window)
{
	checkValid();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(m_programId);
	const geometry::Vector2& windowSize = window->getSize();
	glViewport(0, 0, windowSize.getX(), windowSize.getY());
	
	int i = 0;
	for (std::vector<Texture>::iterator it = m_inputTextures.begin(); it != m_inputTextures.end(); it++)
	{
		const Uniform& textureUniform = getUniform(it->getName());
		textureUniform.setTexture(&*it, i);
		i++;
	}
}

Attribute Program::getAttribute(std::string attributeName)
{
	std::map<std::string, Attribute>::iterator it = m_attributes.find(attributeName);
	
	if (it != m_attributes.end())
		return it->second;
		
	else
	{
		std::cerr << "Warning: attribute '" << attributeName << "' does not exist in '" << m_fragmentShader << "' and '" << m_vertexShader << "'" << std::endl;
		return -1;
	}
}

Uniform Program::getUniform(std::string uniformName)
{
	std::map<std::string, Uniform>::iterator it = m_uniforms.find(uniformName);
	
	if (it != m_uniforms.end())
		return it->second;
		
	else
	{
		std::cerr << "Warning: uniform '" << uniformName << "' does not exist in '" << m_fragmentShader << "' and '" << m_vertexShader << "'" << std::endl;
		return Uniform(-1);
	}
}

void Program::addInputTexture(const Texture& inputTexture)
{
	m_inputTextures.push_back(inputTexture);
}

void Program::checkValid()
{
	if (!m_valid)
	{
		std::cerr << "Fatal error: using invalid shader program" << std::endl;
		exit(1);
	}
}

GLuint Program::compileProgram(GLuint fragmentShaderId, GLuint vertexShaderId)
{
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	return programId;
}

GLuint Program::compileShader(std::string shader, GLuint shaderType)
{
	const GLchar* shaderCode = readCode(shader);
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderCode, NULL);
	delete shaderCode;
	glCompileShader(shaderId);
	return shaderId;
}

void Program::checkProgram(GLuint programId)
{
	GLint result = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);

	if (!result)
	{
		GLsizei infoLogLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar message[infoLogLength];
		glGetProgramInfoLog(programId, infoLogLength, NULL, message);
		std::cerr << "Warning: " << message << std::endl;
		m_valid = false;
	}
}

void Program::checkShader(std::string shaderFile, GLuint shaderId)
{
	GLint result = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		GLsizei infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar message[infoLogLength];
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, message);
		std::cerr << "Warning while loading shader file '" << shaderFile << "' :" << std::endl << message << std::endl;
		m_valid = false;
	}
}

const GLchar* Program::readCode(std::string shader)
{
	std::ifstream file(shader.c_str(), std::ifstream::binary);
	if (!file.is_open())
	{
		std::cerr << "Warning: unable to open shader file '" << shader << "'" << std::endl;
		m_valid = false;
		return NULL;
	}
	
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);
	
	GLchar* code = new GLchar[length + 1];
	file.read(code, length);
	code[length] = '\0';
		
	file.close();
	
	return code;
}

void Program::loadAttributes()
{
	GLint total = -1;
	glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTES, &total); 
	for(GLint i = 0; i < total; i++) {
		GLsizei nameLength;
		GLint size;
		GLenum type = GL_ZERO;
		GLchar name[100];
		glGetActiveAttrib(m_programId, i, sizeof(name) - 1, &nameLength, &size, &type, name);
		name[nameLength] = '\0';
		Attribute location = glGetAttribLocation(m_programId, name);
		m_attributes[name] = location;
	}
}

void Program::loadUniforms()
{
	GLint total = -1;
	glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &total); 
	for(GLint i = 0; i < total; i++) {
		GLsizei nameLength;
		GLint size;
		GLenum type = GL_ZERO;
		GLchar name[100];
		glGetActiveUniform(m_programId, i, sizeof(name) - 1, &nameLength, &size, &type, name);
		name[nameLength] = '\0';
		Attribute location = glGetUniformLocation(m_programId, name);
		m_uniforms[name] = Uniform(location);
	}
}

} // video
} // flat


