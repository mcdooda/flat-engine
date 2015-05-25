#include <iostream>
#include "../geometry/rectangle.h"
#include "sprite.h"

namespace flat
{
namespace util
{

Sprite::Sprite() :
	m_texture(nullptr),
	m_scale(1.f),
	m_modelMatrixIsDirty(true),
	m_isLightCopy(false)
{
	
}

Sprite::~Sprite()
{
	
}

Sprite* Sprite::lightCopy()
{
	Sprite* sprite = new Sprite(*this);
	sprite->m_isLightCopy = true;
	return sprite;
}

void Sprite::draw(const RenderSettings& renderSettings, const geometry::Matrix4& viewMatrix)
{
	renderSettings.textureUniform.setTexture(m_texture);
	renderSettings.colorUniform.setColor(m_color);
	updateModelMatrix();
	renderSettings.modelMatrixUniform.setMatrix4(m_modelMatrix);
	geometry::Rectangle rectangle(m_texture->getSize() / -2.f, m_texture->getSize());
	rectangle.draw(renderSettings.positionAttribute, renderSettings.uvAttribute);
}

void Sprite::updateModelMatrix()
{
	if (m_modelMatrixIsDirty)
	{
		m_modelMatrixIsDirty = false;
		m_modelMatrix.setIdentity();
		m_modelMatrix.rotateZ(m_rotation.z);
		m_modelMatrix.rotateY(m_rotation.y);
		m_modelMatrix.rotateX(m_rotation.x);
		m_modelMatrix.scale(m_scale);
		m_modelMatrix.translate(m_position);
	}
}

} // util
} // flat



