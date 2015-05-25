#include "framebuffer.h"

namespace flat
{
namespace video
{

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &m_fboId);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_fboId);
	for (std::vector<Texture>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		GLuint textureId = it->getTextureId();
		glDeleteTextures(1, &textureId);
	}
}

const Texture& FrameBuffer::addTexture(const std::string& name)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
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

void FrameBuffer::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	glViewport(0, 0, m_size.x, m_size.y);
	
	GLenum bufferList[m_textures.size()];
	for (unsigned int i = 0; i < m_textures.size(); i++)
		bufferList[i] = GL_COLOR_ATTACHMENT0 + i;
		
	glDrawBuffers(m_textures.size(), bufferList);
}

} // video
} // flat



