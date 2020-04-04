#include "chunk.h"
#include "debug/assert.h"

namespace flat::audio
{

Chunk::Chunk(const std::string& filename)
{
    m_chunk = Mix_LoadWAV(filename.c_str());
    FLAT_ASSERT(m_chunk != nullptr);
//TODO error checking
//    if(!m_chunk) {
//        std::cout <<"Mix_LoadWAV: " << Mix_GetError() << std::endl;
//    }
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