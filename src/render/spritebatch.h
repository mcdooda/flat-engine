#ifndef FLAT_RENDER_SPRITEBATCH_H
#define FLAT_RENDER_SPRITEBATCH_H

#include <vector>

#include "video/color.h"
#include "misc/vector.h"
#include "misc/matrix4.h"

namespace flat
{
namespace video
{
class Texture;
}
namespace render
{
class BaseSprite;
struct RenderSettings;

class SpriteBatch
{
public:
	SpriteBatch();

	void clear();
	void add(const BaseSprite& sprite, const Matrix4& transformPositionMatrix);
	void draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const;

	struct Vertex
	{
		Vector3 pos;
		Vector2 uv;
		video::Color color;
		float useColor;
		Vector3 normal;
	};

private:
	std::vector<Vertex> m_vertices;
	const video::Texture* m_texture;
};

} // render
} // flat

#endif // FLAT_RENDER_SPRITEBATCH_H


