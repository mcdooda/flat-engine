#ifndef FLAT_UTIL_ANIMATEDSPRITE_H
#define FLAT_UTIL_ANIMATEDSPRITE_H

#include "sprite.h"

namespace flat
{
namespace util
{

class AnimatedSprite : public Sprite
{
	public:
		AnimatedSprite();
		~AnimatedSprite() override;
		
		void setAtlasSize(int atlasWidth, int atlasHeight);
		inline void setAnimated(bool animated) { m_animated = animated; }
		
		inline void setFrameDuration(float frameDuration) { m_frameDuration = frameDuration; }
		
		void setLine(int line);
		void setColumn(int column);
		
		void update(float currentTime);
		
	private:
		int getAtlasWidth() const;
		int getAtlasHeight() const;
		
		flat::geometry::Vector2 m_tileSizeRatio;
		
		int m_currentLine;
		int m_currentColumn;
		
		float m_lastUpdateTime;
		float m_frameDuration;
		
		bool m_animated : 1;
};

} // util
} // flat

#endif // FLAT_UTIL_ANIMATEDSPRITE_H



