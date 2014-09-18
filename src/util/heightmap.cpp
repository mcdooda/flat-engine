#include "heightmap.h"

namespace flat
{
namespace util
{

HeightMap::HeightMap() :
	m_heightMap(NULL),
	m_bumpMap(NULL)
{
	
}

HeightMap::~HeightMap()
{
	
}

Sprite* HeightMap::lightCopy()
{
	return new HeightMap(*this);
}

void HeightMap::draw(const RenderSettings& renderSettings, const geometry::Matrix4& viewMatrix)
{
	renderSettings.textureUniform.setTexture(m_texture);
	
	if (m_bumpMap != NULL)
		renderSettings.bumpMapUniform.setTexture(m_bumpMap, 1);
	
	updateModelMatrix();
	renderSettings.modelMatrixUniform.setMatrix4(m_modelMatrix);
	
	geometry::Matrix4 normalMatrix = viewMatrix * m_modelMatrix;
	normalMatrix.setInverseTranspose();
	renderSettings.normalMatrixUniform.setMatrix4(normalMatrix);
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// position
	glEnableVertexAttribArray(renderSettings.positionAttribute);
	glVertexAttribPointer(renderSettings.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3d), &m_vertices[0].x);
	
	// normal
	glEnableVertexAttribArray(renderSettings.normalAttribute);
	glVertexAttribPointer(renderSettings.normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3d), &m_vertices[0].nx);

	// uv
	glEnableVertexAttribArray(renderSettings.uvAttribute);
	glVertexAttribPointer(renderSettings.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3d), &m_vertices[0].u);

	// draw
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, m_indices);
	
	// disable position
	glDisableVertexAttribArray(renderSettings.positionAttribute);
	
	// disable position
	glDisableVertexAttribArray(renderSettings.normalAttribute);
	
	// disable uv
	glDisableVertexAttribArray(renderSettings.uvAttribute);
	
	glDisable(GL_DEPTH_TEST);
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

Vertex3d* HeightMap::getVertex(unsigned int x, unsigned int y) const
{
	unsigned int width  = m_heightMap->getSize().getX();
	return &m_vertices[x * width + y];
}

void HeightMap::computeHeightMap()
{
	unsigned int width  = m_heightMap->getSize().getX();
	unsigned int height = m_heightMap->getSize().getY();
	unsigned int numVertices = width * height;
	m_vertices = new Vertex3d[numVertices];
	
	float textureWidth = m_texture->getSize().getX();
	float textureHeight = m_texture->getSize().getY();
	
	unsigned int x, y;
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			Vertex3d* v = getVertex(x, y);
			v->x = ((float) x / (width - 1) - 0.5) * textureWidth;
			v->y = ((float) y / (height - 1) - 0.5) * textureHeight;
			v->z = getHeight(x, y + 1) * textureWidth; // y + 1 ????
			v->u = (float) x / (width - 1);
			v->v = (float) y / (height - 1);
		}
	}
	
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			Vertex3d* v = getVertex(x, y);
			Vertex3d *topVertex, *bottomVertex, *leftVertex, *rightVertex;
			
			// left
			if (x > 0)
				leftVertex = getVertex(x - 1, y);
				
			else
				leftVertex = v;
				
			// right
			if (x < width - 1)
				rightVertex = getVertex(x + 1, y);
				
			else
				rightVertex = v;
				
			// top
			if (y > 0)
				topVertex = getVertex(x, y - 1);
				
			else
				topVertex = v;
			
			// bottom
			if (y < height - 1)
				bottomVertex = getVertex(x, y + 1);
				
			else
				bottomVertex = v;
			
			flat::geometry::Vector3 dx(rightVertex->x - leftVertex->x, 0, rightVertex->z - leftVertex->z);
			flat::geometry::Vector3 dy(0, topVertex->y - bottomVertex->y, topVertex->z - bottomVertex->z);
			flat::geometry::Vector3 normal = dx.crossProduct(dy).normalize();
			//std::cout << "normal = " << normal << " / " << normal.normalize() << std::endl;
			v->nx = normal.getX();
			v->ny = normal.getY();
			v->nz = normal.getZ();
		}
	}

	m_numIndices = (width - 1) * 6 * (height - 1);
	m_indices = new unsigned int[m_numIndices];

	unsigned int i = 0;
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



