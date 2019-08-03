#ifndef FLAT_FILE_SERIALIZE_SERIALIZABLE_H
#define FLAT_FILE_SERIALIZE_SERIALIZABLE_H

namespace flat
{
namespace file
{
namespace serialize
{
class Processor;

class Serializable
{
	public:
		virtual void process(Processor& processor) = 0;
};

} // serialize
} // file
} // flat

#endif // FLAT_FILE_SERIALIZE_SERIALIZABLE_H