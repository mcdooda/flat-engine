#ifndef FLAT_AUDIO_AUDIO_H
#define FLAT_AUDIO_AUDIO_H

#include <string>

#include "audio/music.h"
#include "audio/sample.h"
#include "audio/chunk.h"
#include "resource/weakresourcemanager.h"

namespace flat::audio
{

class Audio
{
	public:
		Audio();
		~Audio();

		void setVolume(float volumeLevel) const;

		inline std::shared_ptr<const Chunk> getChunk(const std::string& fileName) const {
			return m_chunkManager.getResource(fileName); }

	private:
		resource::WeakResourceManager<Chunk, std::string> m_chunkManager;
		static const Music* currentMusic;
};

} // flat::audio

#endif // FLAT_AUDIO_AUDIO_H
