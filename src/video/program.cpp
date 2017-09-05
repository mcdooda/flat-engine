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

	assertValid();
}

void Program::use(const Window& window) const
{
	use(window.getSize());
}

void Program::use(const Vector2& size) const
{
	assertValid();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(m_programId);
	glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));

	int i = 0;
	for (const Texture& texture : m_inputTextures)
	{
		Uniform<Texture> textureUniform = getUniform<Texture>(texture.getName());
		textureUniform.set(&texture, i);
		i++;
	}
}

Attribute Program::getAttribute(const std::string& attributeName) const
{
	std::map<std::string, Attribute>::const_iterator it = m_attributes.find(attributeName);
	
	if (it != m_attributes.end())
		return it->second;
		
	else
	{
		std::cerr << "Warning: attribute '" << attributeName << "' does not exist in '" << m_fragmentShader << "' and '" << m_vertexShader << "'" << std::endl;
		return -1;
	}
}

void Program::addInputTexture(const Texture& inputTexture)
{
	m_inputTextures.push_back(inputTexture);
}

void Program::assertValid() const
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
		std::vector<GLchar> message(infoLogLength);
		glGetProgramInfoLog(programId, infoLogLength, nullptr, message.data());
		std::cerr << "Warning: " << message.data() << std::endl;
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
		std::vector<GLchar> message(infoLogLength);
		glGetShaderInfoLog(shaderId, infoLogLength, nullptr, message.data());
		std::cerr << "Warning while loading shader file '" << shaderFile << "' :" << std::endl << message.data() << std::endl;
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


