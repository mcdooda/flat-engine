#ifndef FLAT_VIDEO_TEXTURE_H
#define FLAT_VIDEO_TEXTURE_H

#include <GL/glew.h>
#include "../geometry/vector2.h"

namespace flat
{
namespace video
{

class Texture
{
	public:
		Texture();
		Texture(GLuint textureId, const geometry::Vector2& size, const std::string& name);
		~Texture();
		
		inline GLuint getTextureId() const { return m_textureId; }
		inline const geometry::Vector2& getSize() const { return m_size; }
		inline const std::string& getName() const { return m_name; }
		
		friend class Pass;
		
	protected:
		GLuint m_textureId;
		geometry::Vector2 m_size;
		std::string m_name;
};

} // video
} // flat

#endif // FLAT_VIDEO_TEXTURE_H


