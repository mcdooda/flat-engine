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

	        virtual void draw(const RenderSettings& renderSettings) const;
	        
	        void setHeightMap(video::FileTexture* heightMap, float multiplier = 0.0017);
	        
		protected:
			float getHeight(unsigned int x, unsigned y) const;
			void computeHeightMap();
			
        protected:
        	video::FileTexture* m_heightMap;
            float m_multiplier;
            
            Vertex3d* m_vertices;
            unsigned int m_numIndices;
            unsigned int* m_indices;
    };

} // util
} // flat

#endif // FLAT_UTIL_HEIGHTMAP_H



