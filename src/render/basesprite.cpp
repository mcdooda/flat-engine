#include "render/basesprite.h"
#include "render/rendersettings.h"

#include "misc/aabb2.h"
#include "video/filetexture.h"

namespace flat
{
namespace render
{

BaseSprite::BaseSprite() :
	m_color(video::Color::WHITE),
	m_normal(0.f, 0.f, 1.f),
	m_rotation(0.f),
	m_position(0.f, 0.f, 0.f),
	m_origin(0.f, 0.f),
	m_scale(1.f),
	m_flipX(false),
	m_flipY(false),
	m_useColor(false),
	m_modelMatrixIsDirty(true)
{
	
}

BaseSprite::~BaseSprite()
{
	
}

void BaseSprite::setTexture(const std::shared_ptr<const video::Texture>& texture)
{
	m_texture = texture;
	const video::Texture* t = texture.get();
	FLAT_ASSERT_MSG(t != nullptr, "Cannot set a null texture on a sprite");
	const Vector2& textureSize = t->getSize();
	m_origin = textureSize / 2.f;
	m_modelMatrixIsDirty = true;
}

void BaseSprite::draw(const RenderSettings& renderSettings, const Matrix4& /*viewMatrix*/) const
{
	const video::Texture* texture = getTexture().get();
	FLAT_ASSERT(texture != nullptr);
	renderSettings.textureUniform.set(texture);
	renderSettings.colorUniform.set(m_color);
	updateModelMatrix();
	renderSettings.modelMatrixUniform.set(m_modelMatrix);
	
	video::Attribute positionAttribute = renderSettings.positionAttribute;
	video::Attribute uvAttribute = renderSettings.uvAttribute;
	
	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), &getVertexPositions()[0].x);
	
	glEnableVertexAttribArray(uvAttribute);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), &getVertexUvs()[0].x);
	
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
	
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(uvAttribute);
}

void BaseSprite::updateModelMatrix() const
{
	if (m_modelMatrixIsDirty)
	{
		m_modelMatrixIsDirty = false;
		m_modelMatrix = Matrix4();
		translateBy(m_modelMatrix, m_position);
		rotateBy(m_modelMatrix, m_rotation);
		scaleBy(m_modelMatrix, m_scale);
		float scaleX = m_flipX ? -1.f : 1.f;
		float scaleY = m_flipY ? -1.f : 1.f;
		scaleBy(m_modelMatrix, Vector2(scaleX, scaleY));
		translateBy(m_modelMatrix, Vector3(-m_origin, 0.f));
	}
}

void BaseSprite::setModelMatrix(const Matrix4& modelMatrix)
{
	// TODO: do better than this
	m_modelMatrixIsDirty = false;
	m_modelMatrix = modelMatrix;
	m_position = flat::Vector3(m_modelMatrix[3]);
	m_rotation = flat::Vector3();
	m_scale = flat::Vector2();
}

void BaseSprite::getAABB(AABB2& aabb) const
{
	const Matrix4& aMatrix = getModelMatrix();
	// TODO: wrong for rotated sprites
	aabb.min = Vector2(aMatrix * Vector4(getVertexPositions()[0], 1.f));
	aabb.max = Vector2(aMatrix * Vector4(getVertexPositions()[3], 1.f));

	if (aabb.min.x > aabb.max.x)
		std::swap(aabb.min.x, aabb.max.x);
	if (aabb.min.y > aabb.max.y)
		std::swap(aabb.min.y, aabb.max.y);

	FLAT_ASSERT(aabb.isValid());
}

void BaseSprite::getPixel(const Vector2& point, video::Color& color) const
{
	AABB2 aabb;
	getAABB(aabb);
	FLAT_ASSERT(aabb.isInside(point));
	FLAT_ASSERT_MSG(dynamic_cast<const video::FileTexture*>(m_texture.get()) != nullptr, "Can only get pixel color from a file texture");
	const video::FileTexture* texture = static_cast<const video::FileTexture*>(m_texture.get());
	const Vector2& textureSize = texture->getSize();

	// TODO: wrong for rotated sprites
	float rx = (point.x - aabb.min.x) / (aabb.max.x - aabb.min.x);
	float ry = (point.y - aabb.min.y) / (aabb.max.y - aabb.min.y);

	if (m_flipX)
	{
		rx = 1.f - rx;
	}

	if (m_flipY)
	{
		ry = 1.f - ry;
	}

	const VertexUvs& vertexUvs = getVertexUvs();

	Vector2 pixelPosition;
	pixelPosition.x = (rx * (vertexUvs[1].x - vertexUvs[0].x) + vertexUvs[0].x) * textureSize.x;
	pixelPosition.y = (ry * (vertexUvs[2].y - vertexUvs[0].y) + vertexUvs[0].y) * textureSize.y;

	texture->getPixel(pixelPosition, color);
}

bool BaseSprite::requiresAlphaBlending() const
{
	return m_texture->requiresAlphaBlending() || m_color.a < 1.f;
}

} // render
} // flat


