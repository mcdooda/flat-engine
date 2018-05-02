#include <cmath>
#include <iostream>
#include "spriteanimator.h"
#include "basesprite.h"
#include "../misc/aabb2.h"
#include "../video/filetexture.h"

namespace flat
{
namespace render
{

SpriteAnimator::SpriteAnimator(BaseSprite* sprite) :
	m_sprite(sprite),
	m_tileSizeRatio(1.f, 1.f),
	m_currentLine(0),
	m_currentColumn(0),
	m_lastUpdateTime(0.f),
	m_frameDuration(1.f),
	m_numFrames(1),
	m_animated(false),
	m_animationStarted(false)
{
	FLAT_ASSERT(sprite != nullptr);
}

SpriteAnimator::~SpriteAnimator()
{
	
}

void SpriteAnimator::setAtlasSize(int atlasWidth, int atlasHeight)
{
	const flat::video::Texture* texture = m_sprite->getTexture().get();
	FLAT_ASSERT(texture != nullptr);
	const flat::Vector2& textureSize = texture->getSize();
	FLAT_ASSERT(0 < atlasWidth  && atlasWidth  < textureSize.x);
	FLAT_ASSERT(0 < atlasHeight && atlasHeight < textureSize.y);
	if (textureSize.x / atlasWidth != floor(textureSize.x / atlasWidth)
		|| textureSize.y / atlasHeight != floor(textureSize.y / atlasHeight))
	{
		const flat::video::FileTexture* fileTexture = dynamic_cast<const flat::video::FileTexture*>(texture);
		std::cerr << "Invalid atlas size (" << atlasWidth << ", " << atlasHeight << ") "
			<< "for texture "
			<< ((fileTexture != nullptr) ? fileTexture->getFileName() + " " : "")
			<< "of size (" << static_cast<int>(textureSize.x) << ", " << static_cast<int>(textureSize.y) << ")" << std::endl;
	}
	m_tileSizeRatio.x = 1.f / atlasWidth;
	m_tileSizeRatio.y = 1.f / atlasHeight;
	setLine(0);
	setColumn(0);

	const float atlasTileWidth = textureSize.x / atlasWidth;
	const float atlasTileHeight = textureSize.y / atlasHeight;
	BaseSprite::VertexPositions& vertexPositions = m_sprite->getVertexPositions();
	vertexPositions[1].x = atlasTileWidth;
	vertexPositions[2].y = atlasTileHeight;
	vertexPositions[3].x = atlasTileWidth;
	vertexPositions[3].y = atlasTileHeight;
	vertexPositions[4].y = atlasTileHeight;
	vertexPositions[5].x = atlasTileWidth;
}

void SpriteAnimator::setAnimated(bool animated)
{
	if (!m_animated && animated)
	{
		m_animationStarted = false;
	}
	m_animated = animated;
}

void SpriteAnimator::setLine(int line)
{
	FLAT_ASSERT(0 <= line && line < getAtlasHeight());
	m_currentLine = line;
	
	// update uv
	float uvy0 = m_tileSizeRatio.y * line;
	float uvy1 = m_tileSizeRatio.y * (line + 1);
	
	BaseSprite::VertexUvs& vertexUvs = m_sprite->getVertexUvs();
	vertexUvs[0].y = uvy0;
	vertexUvs[1].y = uvy0;
	vertexUvs[2].y = uvy1;
	vertexUvs[3].y = uvy1;
	vertexUvs[4].y = uvy1;
	vertexUvs[5].y = uvy0;
}

void SpriteAnimator::setColumn(int column)
{
	FLAT_ASSERT(0 <= column && column < getAtlasWidth());
	m_currentColumn = column;
	
	// update uv
	float uvx0 = m_tileSizeRatio.x * column;
	float uvx1 = m_tileSizeRatio.x * (column + 1);
	
	BaseSprite::VertexUvs& vertexUvs = m_sprite->getVertexUvs();
	vertexUvs[0].x = uvx0;
	vertexUvs[1].x = uvx1;
	vertexUvs[2].x = uvx0;
	vertexUvs[3].x = uvx1;
	vertexUvs[4].x = uvx0;
	vertexUvs[5].x = uvx1;
}

void SpriteAnimator::playAnimation(int line, int numFrames, float frameDuration, int numLoops)
{
	setLine(line);
	setColumn(0);
	FLAT_ASSERT(numFrames <= getAtlasWidth());
	setNumFrames(numFrames);
	setFrameDuration(frameDuration);
	setNumLoops(numLoops);
	setAnimated(true);
	m_animationStarted = false;
}

void SpriteAnimator::update(float currentTime)
{
	if (m_animated)
	{
		if (!m_animationStarted)
		{
			m_animationStarted = true;
			m_lastUpdateTime = currentTime;
		}
		
		if (currentTime - m_lastUpdateTime > m_frameDuration)
		{
			int nextColumn = m_currentColumn + 1;
			if (nextColumn >= m_numFrames)
			{
				nextColumn = 0;
				if (m_numLoops != INFINITE_LOOP)
				{
					--m_numLoops;
					if (m_numLoops == 0)
					{
						nextColumn = m_currentColumn;
						setAnimated(false);
					}
				}
			}
			setColumn(nextColumn);
			m_lastUpdateTime += m_frameDuration;
		}
	}
	else
	{
		m_lastUpdateTime = currentTime;
	}
}

int SpriteAnimator::getAtlasWidth() const
{
	return static_cast<int>(std::round(1.f / m_tileSizeRatio.x));
}

int SpriteAnimator::getAtlasHeight() const
{
	return static_cast<int>(std::round(1.f / m_tileSizeRatio.y));
}

} // render
} // flat



