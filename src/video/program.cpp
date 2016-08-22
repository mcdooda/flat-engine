#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <tuple>
#include "program.h"
#include "../debug/assert.h"

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

void Program::load(const std::string& fragmentShader, const std::string& vertexShader)
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
	FLAT_ASSERT(window);
	assertValid();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(m_programId);
	const Vector2& windowSize = window->getSize();
	glViewport(0, 0, windowSize.x, windowSize.y);
	
	int i = 0;
	for (std::vector<Texture>::iterator it = m_inputTextures.begin(); it != m_inputTextures.end(); it++)
	{
		const Uniform& textureUniform = getUniform(it->getName());
		textureUniform.setTexture(&*it, i);
		i++;
	}
}

Attribute Program::getAttribute(const std::string& attributeName)
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

Uniform Program::getUniform(const std::string& uniformName)
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

void Program::assertValid()
{
	FLAT_ASSERT_MSG(m_valid, "Fatal error: using invalid shader program");
}

GLuint Program::compileProgram(GLuint fragmentShaderId, GLuint vertexShaderId)
{
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	return programId;
}

GLuint Program::compileShader(const std::string& shader, GLuint shaderType)
{
	std::string shaderCode;
	readCode(shader, shaderCode);
	GLuint shaderId = glCreateShader(shaderType);
	const GLchar* p = shaderCode.c_str();
	glShaderSource(shaderId, 1, &p, nullptr);
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
		GLchar message[256]; // TODO fix
		glGetProgramInfoLog(programId, infoLogLength, nullptr, message);
		std::cerr << "Warning: " << message << std::endl;
		m_valid = false;
	}
}

void Program::checkShader(const std::string& shaderFile, GLuint shaderId)
{
	GLint result = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		GLsizei infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar message[256]; // TODO fix
		glGetShaderInfoLog(shaderId, infoLogLength, nullptr, message);
		std::cerr << "Warning while loading shader file '" << shaderFile << "' :" << std::endl << message << std::endl;
		m_valid = false;
	}
}

void Program::readCode(const std::string& shader, std::string& code)
{
	std::ifstream file(shader.c_str(), std::ifstream::binary);
	if (!file.is_open())
	{
		std::cerr << "Warning: unable to open shader file '" << shader << "'" << std::endl;
		m_valid = false;
		code = "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	code = buffer.str();
		
	file.close();
}

void Program::loadAttributes()
{
	GLint total = -1;
	glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTES, &total); 
	for (GLint i = 0; i < total; i++)
	{
		GLsizei nameLength;
		GLint size;
		GLenum type = GL_ZERO;
		GLchar name[128];
		glGetActiveAttrib(m_programId, i, sizeof(name) - 1, &nameLength, &size, &type, name);
		name[nameLength] = '\0';
		Attribute location = glGetAttribLocation(m_programId, name);
		m_attributes.emplace(std::piecewise_construct,
							 std::forward_as_tuple(name),
							 std::forward_as_tuple(location));
	}
}

void Program::loadUniforms()
{
	GLint total = -1;
	glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &total); 
	for (GLint i = 0; i < total; i++)
	{
		GLsizei nameLength;
		GLint size;
		GLenum type = GL_ZERO;
		GLchar name[128];
		glGetActiveUniform(m_programId, i, sizeof(name) - 1, &nameLength, &size, &type, name);
		name[nameLength] = '\0';
		Attribute location = glGetUniformLocation(m_programId, name);
		m_uniforms.emplace(std::piecewise_construct,
						   std::forward_as_tuple(name),
						   std::forward_as_tuple(location));
	}
}

} // video
} // flat


