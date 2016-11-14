#ifndef FLAT_RENDER_ANIMATEDSPRITE_H
#define FLAT_RENDER_ANIMATEDSPRITE_H

#include <iostream>
#include "sprite.h"

namespace flat
{
namespace render
{

class AnimatedSprite : public Sprite
{
	public:
		enum
		{
			INFINITE_LOOP = -1
		};
	
	public:
		AnimatedSprite();
		~AnimatedSprite() override;
		
		void setAtlasSize(int atlasWidth, int atlasHeight);
		
		void setAnimated(bool animated);
		inline bool isAnimated() const { return m_animated; }
		
		inline void setFrameDuration(float frameDuration) { m_frameDuration = frameDuration; }
		
		inline void setNumFrames(int numFrames) { m_numFrames = numFrames; }
		
		inline void setNumLoops(int numLoops) { m_numLoops = numLoops; }
		inline bool hasInfiniteLoop() const { return m_numLoops == INFINITE_LOOP; }
		
		void setLine(int line);
		inline int getCurrentLine() const { return m_currentLine; }

		void setColumn(int column);
		inline int getCurrentColumn() const { return m_currentColumn; }

		inline float getLastUpdateTime() const { return m_lastUpdateTime; }
		
		void playAnimation(int line, int numFrames, float frameDuration, int numLoops = 1);
		
		void update(float currentTime);
		
	private:
		int getAtlasWidth() const;
		int getAtlasHeight() const;
		
		flat::Vector2 m_tileSizeRatio;
		
		int m_currentLine;
		int m_currentColumn;
		
		float m_lastUpdateTime;
		float m_frameDuration;
		int m_numFrames;
		int m_numLoops;
		
		bool m_animated : 1;
		bool m_animationStarted : 1;
};

} // render
} // flat

#endif // FLAT_RENDER_ANIMATEDSPRITE_H



