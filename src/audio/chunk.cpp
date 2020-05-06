#include "chunk.h"
#include "debug/assert.h"
#include <iostream>

namespace flat::audio
{

Chunk::Chunk(const std::string& filename)
{
    m_chunk = Mix_LoadWAV(filename.c_str());
//TODO error checking
    if(!m_chunk) {
        std::cout <<"Mix_LoadWAV error: " << Mix_GetError() << std::endl;
    }
    FLAT_ASSERT(m_chunk != nullptr);
}

Chunk::~Chunk()
{
    Mix_FreeChunk(m_chunk);
}

int Chunk::playChannel(int loops) const
{
    return Mix_PlayChannel(-1, m_chunk, loops);
}

}