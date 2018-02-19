#include "sprite.h"
#include "../video/filetexture.h"
#include "../misc/aabb2.h"

namespace flat
{
namespace render
{

Sprite::Sprite() :
	m_color(video::Color::WHITE),
	m_normal(0.f, 0.f, 1.f),
	m_rotation(0.f),
	m_position(0.f, 0.f),
	m_origin(0.f, 0.f),
	m_scale(1.f),
	m_flipX(false),
	m_flipY(false),
	m_isLightCopy(false),
	m_modelMatrixIsDirty(true)
{
	// pos
	m_vertices[0].pos.x = 0.f;
	m_vertices[0].pos.y = 0.f;
	
	m_vertices[1].pos.x = 1.f; // will change according to the texture size
	m_vertices[1].pos.y = 0.f;

	m_vertices[2].pos.x = 0.f;
	m_vertices[2].pos.y = 1.f; // will change according to the texture size
	
	m_vertices[3].pos.x = 1.f; // will change according to the texture size
	m_vertices[3].pos.y = 1.f; // will change according to the texture size
	
	m_vertices[4].pos.x = 0.f;
	m_vertices[4].pos.y = 1.f; // will change according to the texture size

	m_vertices[5].pos.x = 1.f; // will change according to the texture size
	m_vertices[5].pos.y = 0.f;
	
	// uv
	m_vertices[0].uv.x = 0.f;
	m_vertices[0].uv.y = 0.f;
	
	m_vertices[1].uv.x = 1.f;
	m_vertices[1].uv.y = 0.f;
	
	m_vertices[2].uv.x = 0.f;
	m_vertices[2].uv.y = 1.f;
	
	m_vertices[3].uv.x = 1.f;
	m_vertices[3].uv.y = 1.f;

	m_vertices[4].uv.x = 0.f;
	m_vertices[4].uv.y = 1.f;

	m_vertices[5].uv.x = 1.f;
	m_vertices[5].uv.y = 0.f;
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

void Sprite::setTexture(const std::shared_ptr<const video::Texture>& texture)
{
	m_texture = texture;
	const video::Texture* t = texture.get();
	FLAT_ASSERT_MSG(t != nullptr, "Cannot set a null texture on a sprite");
	const Vector2& textureSize = t->getSize();
	m_origin = textureSize / 2.f;
	m_modelMatrixIsDirty = true;
	m_vertices[1].pos.x = textureSize.x;
	m_vertices[2].pos.y = textureSize.y;
	m_vertices[3].pos.x = textureSize.x;
	m_vertices[3].pos.y = textureSize.y;
	m_vertices[4].pos.y = textureSize.y;
	m_vertices[5].pos.x = textureSize.x;
}

void Sprite::draw(const RenderSettings& renderSettings, const Matrix4& /*viewMatrix*/) const
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
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].pos);
	
	glEnableVertexAttribArray(uvAttribute);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].uv);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(uvAttribute);
}

void Sprite::updateModelMatrix() const
{
	if (m_modelMatrixIsDirty)
	{
		m_modelMatrixIsDirty = false;
		m_modelMatrix = Matrix4();
		translateBy(m_modelMatrix, Vector3(m_position, 0.f));
		rotateBy(m_modelMatrix, m_rotation);
		scaleBy(m_modelMatrix, m_scale);
		float scaleX = m_flipX ? -1.f : 1.f;
		float scaleY = m_flipY ? -1.f : 1.f;
		scaleBy(m_modelMatrix, Vector2(scaleX, scaleY));
		translateBy(m_modelMatrix, Vector3(-m_origin, 0.f));
	}
}

void Sprite::getAABB(AABB2& aabb) const
{
	const Matrix4& aMatrix = getModelMatrix();
	// TODO: wrong for rotated sprites
	aabb.min = Vector2(aMatrix * Vector4(m_vertices[0].pos, 0.f, 1.f));
	aabb.max = Vector2(aMatrix * Vector4(m_vertices[3].pos, 0.f, 1.f));

	if (aabb.min.x > aabb.max.x)
		std::swap(aabb.min.x, aabb.max.x);
	if (aabb.min.y > aabb.max.y)
		std::swap(aabb.min.y, aabb.max.y);

	FLAT_ASSERT(aabb.isValid());
}

void Sprite::getPixel(const Vector2& point, video::Color& color) const
{
	AABB2 aabb;
	getAABB(aabb);
	FLAT_ASSERT(aabb.isInside(point));
	FLAT_ASSERT_MSG(dynamic_cast<const video::FileTexture*>(m_texture.get()) != nullptr, "Can only get pixel color from a file texture");
	const video::FileTexture* texture = static_cast<const video::FileTexture*>(m_texture.get());

	// TODO: wrong for rotated sprites
	float rx = (point.x - aabb.min.x) / (aabb.max.x - aabb.min.x);
	float ry = 1.f - (point.y - aabb.min.y) / (aabb.max.y - aabb.min.y);

	if (m_flipX)
		rx = 1.f - rx;

	if (m_flipY)
		ry = 1.f - ry;

	Vector2 pixelPosition(
		rx * texture->getSize().x,
		ry * texture->getSize().y
	);
	texture->getPixel(pixelPosition, color);
}

} // render
} // flat



