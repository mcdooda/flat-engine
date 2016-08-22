#ifndef FLAT_UTIL_HEIGHTMAP_H
#define FLAT_UTIL_HEIGHTMAP_H

#include "sprite.h"
#include "../video/filetexture.h"

namespace flat
{
namespace util
{

	struct Vertex3d
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};

	class HeightMap : public Sprite
	{
		public:
			HeightMap();
			~HeightMap() override;
			
			Sprite* lightCopy() override;

			void draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const override;
			
			void setHeightMap(std::shared_ptr<const video::FileTexture> heightMap, float multiplier = 0.0017);
			
			inline void setBumpMap(std::shared_ptr<const video::FileTexture> bumpMap) { m_bumpMap = bumpMap; }
			
		protected:
			float getHeight(unsigned int x, unsigned int y) const;
			Vertex3d* getVertex(unsigned int x, unsigned int y) const;
			void computeHeightMap();
			
		protected:
			std::shared_ptr<const video::FileTexture> m_heightMap;
			std::shared_ptr<const video::FileTexture> m_bumpMap;
			Vertex3d* m_vertices;
			unsigned int* m_indices;
			unsigned int m_numIndices;
			float m_multiplier;
	};

} // util
} // flat

#endif // FLAT_UTIL_HEIGHTMAP_H



