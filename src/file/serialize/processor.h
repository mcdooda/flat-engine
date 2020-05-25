#ifndef FLAT_FILE_SERIALIZE_PROCESSOR_H
#define FLAT_FILE_SERIALIZE_PROCESSOR_H

#include <string>
#include <vector>
#include <filesystem>

#include "misc/vector.h"

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

		virtual void process(bool& value) = 0;

		virtual void process(std::int8_t& value) = 0;
		virtual void process(std::int16_t& value) = 0;
		virtual void process(std::int32_t& value) = 0;
		virtual void process(std::uint8_t& value) = 0;
		virtual void process(std::uint16_t& value) = 0;
		virtual void process(std::uint32_t& value) = 0;

		virtual void process(float& value) = 0;
		virtual void process(double& value) = 0;

		virtual void process(flat::Vector2& value) = 0;
		virtual void process(flat::Vector2i& value) = 0;
		virtual void process(flat::Vector3& value) = 0;
		virtual void process(flat::Vector3i& value) = 0;

		virtual void process(std::string& value) = 0;
		virtual void process(std::filesystem::path& value) = 0;

		template <typename SizeType = std::uint32_t, class T>
		void process(std::vector<T>& value);
};

template <typename SizeType, class T>
void Processor::process(std::vector<T>& value)
{
	if (isWriting())
	{
		SizeType size = static_cast<SizeType>(value.size());
		process(size);
		for (T& item : value)
		{
			process(item);
		}
	}
	else
	{
		SizeType size = 0;
		process(size);
		value.resize(size);
		for (SizeType i = 0; i < size; ++i)
		{
			process(value[i]);
		}
	}
}

} // serialize
} // file
} // flat

#endif // FLAT_FILE_SERIALIZE_PROCESSOR_H