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
}

std::shared_ptr<const Texture> FrameBuffer::addTexture(const std::string& name)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(m_size.x), static_cast<GLsizei>(m_size.y), 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	GLenum attachment = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + m_textures.size());
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	std::shared_ptr<const Texture> texture = std::make_shared<const Texture>(textureId, m_size, name);
	m_textures.push_back(texture);
	return texture;
}

void FrameBuffer::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	glViewport(0, 0, static_cast<GLsizei>(m_size.x), static_cast<GLsizei>(m_size.y));
	
	const GLsizei numTextures = static_cast<GLsizei>(m_textures.size());
	std::vector<GLenum> bufferList(numTextures);
	for (GLsizei i = 0; i < numTextures; i++)
		bufferList[i] = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i);
		
	glDrawBuffers(numTextures, bufferList.data());
}

} // video
} // flat



