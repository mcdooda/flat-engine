#include "render/spritebatch.h"
#include "render/basesprite.h"
#include "render/rendersettings.h"
#include "misc/matrix4.h"

namespace flat::render
{

SpriteBatch::SpriteBatch() :
	m_texture(nullptr)
{
	m_vertices.reserve(1024 * 1024 * 4);
}

void SpriteBatch::clear()
{
	m_vertices.clear();
	m_texture = nullptr;
}

void SpriteBatch::add(const BaseSprite& sprite, const Matrix4& transformPositionMatrix)
{
	if (m_vertices.empty())
	{
		FLAT_ASSERT(m_texture == nullptr);
		m_texture = sprite.getTexture().get();
	}
#ifdef FLAT_DEBUG
	else
	{
		FLAT_ASSERT(m_texture == sprite.getTexture().get());
	}
#endif
	FLAT_ASSERT(m_texture != nullptr);

	//const Matrix4& transform = sprite.getModelMatrix();
	Matrix4 transform;
	translateBy(transform, Vector3(transformPositionMatrix * Vector4(sprite.getPosition(), 1.f)));
	rotateBy(transform, sprite.getRotation());
	scaleBy(transform, sprite.getScale());
	float scaleX = sprite.getFlipX() ? -1.f : 1.f;
	float scaleY = sprite.getFlipY() ? -1.f : 1.f;
	scaleBy(transform, Vector2(scaleX, scaleY));
	translateBy(transform, Vector3(-sprite.getOrigin(), 0.f));

	const video::Color& color = sprite.getColor();
	const float useColor = sprite.getUseColor() ? 1.f : 0.f;
	const Vector3& normal = sprite.getNormal();
	const BaseSprite::VertexPositions& vertexPositions = sprite.getVertexPositions();
	const BaseSprite::VertexUvs& vertexUvs = sprite.getVertexUvs();
	Vector4 pos2d(0.f, 0.f, 0.f, 1.f);
	Vector4 pos(0.f, 0.f, 0.f, 1.f);
	for (int i = 0; i < BaseSprite::NUM_VERTICES; ++i)
	{
		SpriteBatch::Vertex& spriteBatchVertex = m_vertices.emplace_back();
		pos2d.x = vertexPositions[i].x;
		pos2d.y = vertexPositions[i].y;
		pos = transform * pos2d;
		spriteBatchVertex.pos.x = pos.x;
		spriteBatchVertex.pos.y = pos.y;
		spriteBatchVertex.pos.z = pos.z;
		spriteBatchVertex.uv = vertexUvs[i];
		spriteBatchVertex.color = color;
		spriteBatchVertex.useColor = useColor;
		spriteBatchVertex.normal = normal;
	}
}

void SpriteBatch::draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const
{
	FLAT_ASSERT(m_texture != nullptr);
	renderSettings.textureUniform.set(m_texture);

	const video::Attribute positionAttribute = renderSettings.positionAttribute;
	const video::Attribute normalAttribute = renderSettings.normalAttribute;
	const video::Attribute uvAttribute = renderSettings.uvAttribute;
	const video::Attribute colorAttribute = renderSettings.colorAttribute;
	const video::Attribute useColorAttribute = renderSettings.useColorAttribute;

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].pos);

	glEnableVertexAttribArray(normalAttribute);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].normal);

	glEnableVertexAttribArray(uvAttribute);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].uv);

	glEnableVertexAttribArray(colorAttribute);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].color);

	glEnableVertexAttribArray(useColorAttribute);
	glVertexAttribPointer(useColorAttribute, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].useColor);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(normalAttribute);
	glDisableVertexAttribArray(uvAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(useColorAttribute);
}

} // flat::render

