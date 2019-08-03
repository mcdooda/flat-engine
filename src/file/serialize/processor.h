#ifndef FLAT_FILE_SERIALIZE_PROCESSOR_H
#define FLAT_FILE_SERIALIZE_PROCESSOR_H

#include <string>
#include <vector>

namespace flat
{
namespace file
{
namespace serialize
{
class Serializable;

class Processor
{
	public:
		virtual bool isWriting() const = 0;
		inline bool isReading() const { return !isWriting(); }

		void process(Serializable& serializable);

		virtual void process(std::int8_t& value) = 0;
		virtual void process(std::int16_t& value) = 0;
		virtual void process(std::int32_t& value) = 0;
		virtual void process(std::uint8_t& value) = 0;
		virtual void process(std::uint16_t& value) = 0;
		virtual void process(std::uint32_t& value) = 0;

		virtual void process(float& value) = 0;
		virtual void process(double& value) = 0;

		virtual void process(std::string& value) = 0;

		template <class T>
		void process(std::vector<T>& value);
};

template <class T>
void Processor::process(std::vector<T>& value)
{
	if (isWriting())
	{
		std::uint32_t size = static_cast<std::uint32_t>(value.size());
		process(size);
		for (T& item : value)
		{
			process(item);
		}
	}
	else
	{
		std::uint32_t size = 0;
		process(size);
		value.resize(size);
		for (std::uint32_t i = 0; i < size; ++i)
		{
			process(value[i]);
		}
	}
}

} // serialize
} // file
} // flat

#endif // FLAT_FILE_SERIALIZE_PROCESSOR_H