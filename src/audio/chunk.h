#ifndef FLAT_AUDIO_CHUNK_H
#define FLAT_AUDIO_CHUNK_H

#include <string>
#include <SDL2/SDL_mixer.h>

namespace flat::audio
{

class Chunk {
public:
    Chunk(const std::string& filename);
    ~Chunk();

    int playChannel(int loops) const;

private:
    Mix_Chunk* m_chunk;
};

}

#endif // FLAT_AUDIO_CHUNK_H