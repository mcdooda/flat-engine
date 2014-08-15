#include "pass.h"

namespace engine
{
namespace video
{

Pass::Pass() : Program()
{
	glGenFramebuffers(1, &m_fboId);
}

Pass::~Pass()
{
	glDeleteFramebuffers(1, &m_fboId);
	for (std::vector<Texture>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		GLuint textureId = it->getTextureId();
		glDeleteTextures(1, &textureId);
	}
}

void Pass::load(const geometry::Vector2& size, std::string fragmentShader, std::string vertexShader)
{
	m_size = size;
	Program::load(fragmentShader, vertexShader);
}

const Texture& Pass::addOutputTexture(std::string name)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.getX(), m_size.getY(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_textures.size(), GL_TEXTURE_2D, textureId, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	m_textures.push_back(Texture(textureId, m_size, name));
	return m_textures.back();
}

void Pass::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	glViewport(0, 0, m_size.getX(), m_size.getY());
	
	GLenum bufferlist[m_textures.size()];
	for (unsigned int i = 0; i < m_textures.size(); i++)
		bufferlist[i] = GL_COLOR_ATTACHMENT0 + i;
		
	glDrawBuffers(m_textures.size(), bufferlist);
	
	glUseProgram(m_programId);
}

} // video
} // engine


