#include "rendersettings.h"

namespace flat
{
namespace render
{

std::uint32_t RenderSettings::getHash() const
{
	return ((((((((((((((((
		  textureUniform.getHash() * 7)
		+ textureGivenUniform.getHash()) * 11)
		+ colorUniform.getHash()) * 13)
		+ secondaryColorUniform.getHash()) * 17
		+ bumpMapUniform.getHash()) * 19
		+ modelMatrixUniform.getHash()) * 23
		+ normalMatrixUniform.getHash()) * 29)
		+ viewProjectionMatrixUniform.getHash() * 31)
		+ positionAttribute * 37)
		+ normalAttribute * 41)
		+ uvAttribute * 43)
		+ colorAttribute * 47)
		+ depthAttribute * 53);
}

} // render
} // flat


