#include "spritebatch.h"
#include "sprite.h"

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

void SpriteBatch::add(const Sprite& sprite)
{
	if (m_numVertices == 0)
	{
		FLAT_ASSERT(m_texture == nullptr);
		m_texture = sprite.getTexture().get();
	}
	FLAT_DEBUG_ONLY(
	else
	{
		FLAT_ASSERT(m_texture == sprite.getTexture().get());
	}
	)
	FLAT_ASSERT(m_texture != nullptr);

	const Matrix4& transform = sprite.getModelMatrix();
	const video::Color& color = sprite.getColor();
	const int numVertices = 6;
	const Sprite::Vertex* vertices = sprite.m_vertices.data();
	for (int i = 0; i < numVertices; ++i)
	{
		const Sprite::Vertex* spriteVertex = vertices + i;
		FLAT_ASSERT(m_numVertices + 1 > 0);
		SpriteBatch::Vertex& spriteBatchVertex = m_vertices[m_numVertices++];
		spriteBatchVertex.pos = Vector2(transform * Vector4(spriteVertex->pos, 0.f, 1.f));
		spriteBatchVertex.uv = spriteVertex->uv;
		spriteBatchVertex.color = color;
	}
}

void SpriteBatch::draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const
{
	FLAT_ASSERT(m_texture != nullptr);
	renderSettings.textureUniform.set(m_texture);

	const video::Attribute positionAttribute = renderSettings.positionAttribute;
	const video::Attribute uvAttribute = renderSettings.uvAttribute;
	const video::Attribute colorAttribute = renderSettings.colorAttribute;

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].pos);

	glEnableVertexAttribArray(uvAttribute);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].uv);

	glEnableVertexAttribArray(colorAttribute);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].color);

	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(uvAttribute);
	glDisableVertexAttribArray(colorAttribute);
}

} // render
} // flat


