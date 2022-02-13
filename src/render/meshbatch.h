#ifndef FLAT_RENDER_MESHBATCH_H
#define FLAT_RENDER_MESHBATCH_H

#include <array>

#include "misc/matrix4.h"
#include "misc/vector.h"
#include "video/color.h"

namespace flat::video
{
	class Texture;
}

namespace flat::render
{
class Mesh;
struct RenderSettings;

class MeshBatch
{
public:
	MeshBatch();

	void clear();
	void add(const Mesh& mesh);
	void draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const;

	struct Vertex
	{
		Vector3 pos;
		Vector2 uv;
		video::Color color;
		float useColor;
	};

private:
	std::array<Vertex, 1024 * 1024 * 4> m_vertices;
	GLsizei m_numVertices;
	const video::Texture* m_texture;
};

} // flat::render

#endif // FLAT_RENDER_MESHBATCH_H
