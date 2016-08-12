#include "color.h"

namespace flat
{
namespace video
{

Color Color::WHITE;
Color Color::BLACK = Color(0.0f);
Color Color::RED   = Color(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
Color Color::BLUE  = Color(0.0f, 0.0f, 1.0f, 1.0f);

Color::Color() :
	m_r(1.0f),
	m_g(1.0f),
	m_b(1.0f),
	m_a(1.0f)
{
	
}

Color::Color(const Color& color) :
	m_r(color.m_r),
	m_g(color.m_g),
	m_b(color.m_b),
	m_a(color.m_a)
{
	
}

Color::Color(AttributeType r, AttributeType g, AttributeType b, AttributeType a) :
	m_r(r),
	m_g(g),
	m_b(b),
	m_a(a)
{

}

Color::Color(AttributeType gray) :
	m_r(gray),
	m_g(gray),
	m_b(gray),
	m_a(1.0f)
{
	
}

} // video
} // flat


