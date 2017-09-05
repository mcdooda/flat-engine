#ifndef FLAT_VIDEO_FRAMEBUFFER_H
#define FLAT_VIDEO_FRAMEBUFFER_H

#include <vector>
#include <memory>
#include "texture.h"

namespace flat
{
namespace video
{

class FrameBuffer
{
	public:
		FrameBuffer();
		~FrameBuffer();
		
		inline void setSize(const Vector2& size) { m_size = size; }
		inline const Vector2& getSize() const { return m_size; }
		
		std::shared_ptr<const Texture> addTexture(const std::string& name);
		
		void use();
		
	protected:
		Vector2 m_size;
		std::vector<std::shared_ptr<const Texture>> m_textures;
		GLuint m_fboId;
};

} // video
} // flat

#endif // FLAT_VIDEO_FRAMEBUFFER_H



