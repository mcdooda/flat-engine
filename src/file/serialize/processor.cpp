#include "processor.h"
#include "serializable.h"

namespace flat
{
namespace file
{
namespace serialize
{

void Processor::process(Serializable& serializable)
{
	serializable.process(*this);
}

} // serialize
} // file
} // flat