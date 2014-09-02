#include <iostream>
#include "../geometry/rectangle.h"
#include "sprite.h"

namespace flat
{
namespace util
{

Sprite::Sprite() :
	m_texture(NULL),
	m_rotation(0),
	m_scale(1.f)
{
	
}

Sprite::~Sprite()
{
	
}

void Sprite::draw(const RenderSettings& renderSettings) const
{
	static const geometry::Rectangle rectangle(m_texture->getSize() / -2.f, m_texture->getSize());
	renderSettings.textureUniform.setTexture(m_texture);
	geometry::Matrix4 modelMatrix;
	modelMatrix.rotateZ(m_rotation);
	modelMatrix.scale(m_scale);
	modelMatrix.translate(m_position);
	renderSettings.modelMatrixUniform.setMatrix4(modelMatrix);
	rectangle.draw(renderSettings.positionAttribute, renderSettings.uvAttribute);
}

} // util
} // flat



