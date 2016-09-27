#ifndef FLAT_UTIL_SPRITEBATCH_H
#define FLAT_UTIL_SPRITEBATCH_H

#include <vector>
#include <array>
#include "../video/color.h"
#include "../misc/vector.h"
#include "../misc/matrix4.h"

namespace flat
{
namespace video
{
class Texture;
}
namespace util
{
class Sprite;
struct RenderSettings;

class SpriteBatch
{
public:
	SpriteBatch();

	void clear();
	void add(const Sprite& sprite);
	void draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const;

	struct Vertex
	{
		Vector2 pos;
		Vector2 uv;
		video::Color color;
	};

private:
	std::array<Vertex, 2000> m_vertices;
	GLsizei m_numVertices;
	const video::Texture* m_texture;
};

} // util
} // flat

#endif // FLAT_UTIL_SPRITEBATCH_H


