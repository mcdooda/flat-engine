#include "video/render.h"

namespace flat
{
namespace video
{

geometry::Rectangle Render::screenRectangle(Vector2(-1.f, -1.f), Vector2(2.f, 2.f));

Render::Render() : Program()
{

}

Render::~Render()
{

}

void Render::load(const std::string& fragmentShader, const std::string& vertexShader)
{
	Program::load(fragmentShader, vertexShader);
	m_positionAttribute = getAttribute("position");
	m_uvAttribute = getAttribute("uv");
}

void Render::draw()
{
	screenRectangle.draw(m_positionAttribute, m_uvAttribute);
}

} // video
} // flat


