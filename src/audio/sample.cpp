#include "sample.h"
#include "misc/std.h"

namespace flat::audio {
Sample::Sample(std::shared_ptr<const Chunk> chunk):
    m_chunk(chunk)
{
}

Sample::~Sample()
{
    stop();
}

void Sample::play(int loops)
{
    m_channel = m_chunk->playChannel(loops);
    FLAT_ASSERT(m_channel != -1);
}

void Sample::stop()
{
    Mix_HaltChannel(m_channel);
}

void Sample::pause()
{
    Mix_Pause(m_channel);
}

void Sample::resume()
{
    Mix_Resume(m_channel);
}

bool Sample::isPaused() const
{
    return Mix_Paused(m_channel);
}

bool Sample::isPlaying() const
{
    return Mix_Playing(m_channel);
}

void Sample::setPosition(const flat::Vector2& position) const
{
    const float angle = atan2(position.x, position.y);
    const int distance = std::sqrt(flat::square(position.x) + flat::square(position.y));
    Mix_SetPosition(m_channel, angle, distance * 255);
}

}