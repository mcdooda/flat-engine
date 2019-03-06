#ifndef FLAT_VIDEO_TEXTURE_H
#define FLAT_VIDEO_TEXTURE_H

#include <string>
#include <GL/glew.h>

#include "misc/vector.h"

namespace flat
{
namespace video
{

class Texture
{
	friend class Pass;
	public:
		Texture();
		Texture(GLuint textureId, const Vector2& size, const std::string& name);
		virtual ~Texture();

		void clear();
		
		static void open()
		{
			
		}

		static void close()
		{
			
		}
		
		inline GLuint getTextureId() const { return m_textureId; }
		inline const Vector2& getSize() const { return m_size; }
		inline const std::string& getName() const { return m_name; }

		inline std::uint32_t getHash() const { return static_cast<std::uint32_t>(m_textureId); }
		
		inline bool requiresAlphaBlending() const { return m_requiresAlphaBlending; }
		
	protected:
		GLuint m_textureId;
		Vector2 m_size;
		std::string m_name;
		bool m_requiresAlphaBlending;
};

} // video
} // flat

#endif // FLAT_VIDEO_TEXTURE_H


