#include "heightmap.h"

namespace flat
{
namespace util
{

HeightMap::HeightMap()
{
	
}

HeightMap::~HeightMap()
{
	
}

void HeightMap::draw(const RenderSettings& renderSettings) const
{
	
}

void HeightMap::setHeightMap(video::FileTexture* heightMap, float multiplier)
{
	m_heightMap = heightMap;
	m_multiplier = multiplier;
	computeHeightMap();
}

float HeightMap::getHeight(unsigned int x, unsigned int y) const
{
	video::Color color = m_heightMap->getPixel(geometry::Vector2(x, y));
	return (float) (color.getR() + color.getG() + color.getB()) / 3.f * m_multiplier;
}

void HeightMap::computeHeightMap()
{
	unsigned int width  = m_heightMap->getSize().getX();
	unsigned int height = m_heightMap->getSize().getY();
	unsigned int numVertices = width * height;
	m_vertices = new Vertex3d[numVertices];
	
	float textureWidth = m_texture->getSize().getX();
	float textureHeight = m_texture->getSize().getY();
	
	unsigned int x, y, i = 0;
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			Vertex3d* v = &m_vertices[i];
			v->x = ((float) x / (width - 1) - 0.5) * textureWidth;
			v->y = ((float) y / (height - 1) - 0.5) * textureHeight;
			v->z = getHeight(x, y) * textureWidth;
			v->u = (float) x / (width - 1);
			v->v = (float) y / (height - 1);
			i++;
		}
	}
	

	m_numIndices = (width - 1) * 6 * (height - 1);
	m_indices = new unsigned int[m_numIndices];

	i = 0;
	for (y = 0; y < height - 1; y++)
	{
		for (x = 0; x < width - 1; x++)
		{
			if (x < width / 2)
			{
				m_indices[i++] = y * width + x;
				m_indices[i++] = (y + 1) * width + x;
				m_indices[i++] = (y + 1) * width + x + 1;

				m_indices[i++] = (y + 1) * width + x + 1;
				m_indices[i++] = y * width + x + 1;
				m_indices[i++] = y * width + x;
			}
			else
			{
				m_indices[i++] = (y + 1) * width + x;
				m_indices[i++] = y * width + x;
				m_indices[i++] = y * width + x + 1;

				m_indices[i++] = y * width + x + 1;
				m_indices[i++] = (y + 1) * width + x + 1;
				m_indices[i++] = (y + 1) * width + x;
			}
		}
	}
}

} // util
} // flat



