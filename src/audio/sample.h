#ifndef FLAT_AUDIO_SAMPLE_H
#define FLAT_AUDIO_SAMPLE_H

#include "chunk.h"
#include "misc/vector.h"

namespace flat::audio
{

class Sample
{
public:
    Sample(std::shared_ptr<const Chunk> chunk);
    ~Sample();

    void play(int loops = 0);
    void stop();

    void pause();
    void resume();
    bool isPaused() const;

    void setPosition(const flat::Vector2& position) const;

private:
    std::shared_ptr<const Chunk> m_chunk;
    int m_channel;
};

}

#endif // FLAT_AUDIO_SAMPLE_H
