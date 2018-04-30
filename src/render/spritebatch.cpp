#include "spritebatch.h"
#include "basesprite.h"
#include "rendersettings.h"

namespace flat
{
namespace render
{

SpriteBatch::SpriteBatch() :
	m_texture(nullptr)
{

}

void SpriteBatch::clear()
{
	m_numVertices = 0;
	m_texture = nullptr;
}

void SpriteBatch::add(const BaseSprite& sprite)
{
	if (m_numVertices == 0)
	{
		FLAT_ASSERT(m_texture == nullptr);
		m_texture = sprite.getTexture().get();
	}
#ifdef FLAT_DEBUG
	else
	{
		FLAT_ASSERT(m_numVertices > 0 && m_numVertices + BaseSprite::NUM_VERTICES < m_vertices.size());
		FLAT_ASSERT(m_texture == sprite.getTexture().get());
	}
#endif
	FLAT_ASSERT(m_texture != nullptr);

	const Matrix4& transform = sprite.getModelMatrix();
	const video::Color& color = sprite.getColor();
	const Vector3& normal = sprite.getNormal();
	const float depth = sprite.getDepth();
	const BaseSprite::VertexPositions& vertexPositions = sprite.getVertexPositions();
	const BaseSprite::VertexUvs& vertexUvs = sprite.getVertexUvs();
	for (int i = 0; i < BaseSprite::NUM_VERTICES; ++i)
	{
		FLAT_ASSERT(m_numVertices + 1 > 0);
		SpriteBatch::Vertex& spriteBatchVertex = m_vertices[m_numVertices++];
		FLAT_ASSERT(m_numVertices > 0);
		spriteBatchVertex.pos = Vector2(transform * Vector4(vertexPositions[i], 0.f, 1.f));
		spriteBatchVertex.uv = vertexUvs[i];
		spriteBatchVertex.color = color;
		spriteBatchVertex.normal = normal;
		spriteBatchVertex.depth = depth;
	}
}

void SpriteBatch::draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const
{
	FLAT_ASSERT(m_texture != nullptr);
	renderSettings.textureUniform.set(m_texture);

	const video::Attribute positionAttribute = renderSettings.positionAttribute;
	const video::Attribute uvAttribute = renderSettings.uvAttribute;
	const video::Attribute colorAttribute = renderSettings.colorAttribute;
	const video::Attribute normalAttribute = renderSettings.normalAttribute;
	const video::Attribute depthAttribute = renderSettings.depthAttribute;

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].pos);

	glEnableVertexAttribArray(uvAttribute);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].uv);

	glEnableVertexAttribArray(colorAttribute);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].color);

	glEnableVertexAttribArray(normalAttribute);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].normal);

	glEnableVertexAttribArray(depthAttribute);
	glVertexAttribPointer(depthAttribute, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].depth);

	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(uvAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(normalAttribute);
	glDisableVertexAttribArray(depthAttribute);
}

} // render
} // flat


