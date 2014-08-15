#include "render.h"

namespace engine
{
namespace video
{

geometry::Rectangle Render::screenRectangle(geometry::Vector2(-1.0f, -1.0f), geometry::Vector2(2.0f, 2.0f));

Render::Render() : Program()
{

}

Render::~Render()
{

}

void Render::load(std::string fragmentShader, std::string vertexShader)
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
} // engine


