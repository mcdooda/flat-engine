#ifndef FLAT_NETWORK_COMMON_NETWORKEVENT_H
#define FLAT_NETWORK_COMMON_NETWORKEVENT_H

#include <cstdint>

namespace flat
{
namespace network
{
namespace common
{
class NetworkProcessor;

class NetworkEvent
{
	public:
		using Delimiter = std::uint8_t;

	public:

		static void setDelimiter(Delimiter delimiter);
		static Delimiter getDelimiter();

		virtual bool process(NetworkProcessor& processor) = 0;

		virtual bool isValid() const;

	private:
		static Delimiter delimiter;
};

} // common
} // network
} // flat


#endif // FLAT_NETWORK_COMMON_NETWORKEVENT_H


