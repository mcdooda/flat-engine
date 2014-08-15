#ifndef FLAT_VIDEO_PASS_H
#define FLAT_VIDEO_PASS_H

#include <GL/glew.h>
#include "program.h"

namespace flat
{
namespace video
{

class Pass : public Program
{
	public:
		Pass();
		virtual ~Pass();
		
		void load(const geometry::Vector2& size, std::string fragmentShader, std::string vertexShader);
		
		const Texture& addOutputTexture(std::string name);
		
		void use();
		
	private:
		geometry::Vector2 m_size;
		std::vector<Texture> m_textures;
		GLuint m_fboId;
};

} // video
} // flat

#endif // FLAT_VIDEO_PASS_H


