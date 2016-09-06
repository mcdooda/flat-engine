#include "heightmap.h"
#include "../memory/memory.h"

namespace flat
{
namespace util
{

HeightMap::HeightMap() :
	m_heightMap(nullptr),
	m_bumpMap(nullptr),
	m_vertices(nullptr),
	m_indices(nullptr)
{
	
}

HeightMap::~HeightMap()
{
	if (!m_isLightCopy)
	{
		FLAT_DELETE_ARRAY(m_vertices);
		FLAT_DELETE_ARRAY(m_indices);
	}
}

Sprite* HeightMap::lightCopy()
{
	HeightMap* heightMap = new HeightMap(*this);
	heightMap->m_isLightCopy = true;
	return heightMap;
}

void HeightMap::draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const
{
	const flat::video::Texture* texture = getTexture().get();
	renderSettings.textureUniform.set(texture);
	renderSettings.colorUniform.set(m_color);
	
	if (m_bumpMap != nullptr)
		renderSettings.bumpMapUniform.set(m_bumpMap.get(), 1);
	
	updateModelMatrix();
	renderSettings.modelMatrixUniform.set(m_modelMatrix);
	
	Matrix4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * m_modelMatrix));
	renderSettings.normalMatrixUniform.set(normalMatrix);
	
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

void HeightMap::setHeightMap(const std::shared_ptr<const video::FileTexture>& heightMap, float multiplier)
{
	m_heightMap = heightMap;
	m_multiplier = multiplier;
	computeHeightMap();
}

float HeightMap::getHeight(unsigned int x, unsigned int y) const
{
	video::Color color = m_heightMap->getPixel(Vector2(static_cast<float>(x), static_cast<float>(y)));
	return static_cast<float>(color.r + color.g + color.b) / 3.f * m_multiplier;
}

Vertex3d* HeightMap::getVertex(unsigned int x, unsigned int y) const
{
	unsigned int width = static_cast<unsigned int>(m_heightMap->getSize().x);
	return &m_vertices[x * width + y];
}

void HeightMap::computeHeightMap()
{
	const unsigned int width = static_cast<unsigned int>(m_heightMap->getSize().x);
	const unsigned int height = static_cast<unsigned int>(m_heightMap->getSize().y);
	const unsigned int numVertices = width * height;
	m_vertices = new Vertex3d[numVertices];
	
	const flat::video::Texture* texture = getTexture().get();
	float textureWidth = texture->getSize().x;
	float textureHeight = texture->getSize().y;
	
	unsigned int x, y;
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			Vertex3d* v = getVertex(x, y);
			v->x = (static_cast<float>(x) / (width - 1) - 0.5f) * textureWidth;
			v->y = (static_cast<float>(y) / (height - 1) - 0.5f) * textureHeight;
			v->z = getHeight(x, y + 1) * textureWidth; // y + 1 ????
			v->u = static_cast<float>(x) / (width - 1);
			v->v = static_cast<float>(y) / (height - 1);
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
			
			flat::Vector3 dx(rightVertex->x - leftVertex->x, 0, rightVertex->z - leftVertex->z);
			flat::Vector3 dy(0, topVertex->y - bottomVertex->y, topVertex->z - bottomVertex->z);
			flat::Vector3 normal = glm::normalize(glm::cross(dx, dy));
			//std::cout << "normal = " << normal << " / " << normal.normalize() << std::endl;
			v->nx = normal.x;
			v->ny = normal.y;
			v->nz = normal.z;
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



