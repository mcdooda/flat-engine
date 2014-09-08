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
            virtual ~HeightMap();

	        virtual void draw(const RenderSettings& renderSettings, const geometry::Matrix4& viewMatrix);
	        
	        void setHeightMap(video::FileTexture* heightMap, float multiplier = 0.0017);
	        
	        inline void setBumpMap(video::FileTexture* bumpMap) { m_bumpMap = bumpMap; }
	        
		protected:
			float getHeight(unsigned int x, unsigned int y) const;
			Vertex3d* getVertex(unsigned int x, unsigned int y) const;
			void computeHeightMap();
			
        protected:
        	video::FileTexture* m_heightMap;
            float m_multiplier;
            
            video::FileTexture* m_bumpMap;
            
            Vertex3d* m_vertices;
            unsigned int m_numIndices;
            unsigned int* m_indices;
    };

} // util
} // flat

#endif // FLAT_UTIL_HEIGHTMAP_H



