#ifndef FLAT_UTIL_RENDERSETTINGS_H
#define FLAT_UTIL_RENDERSETTINGS_H

#include "../video/uniform.h"
#include "../video/attribute.h"

namespace flat
{
namespace util
{

struct RenderSettings
{
	video::Uniform textureUniform;
	video::Uniform textureGivenUniform;
	video::Uniform colorUniform;
	video::Uniform secondaryColorUniform;
	video::Uniform bumpMapUniform;
	video::Uniform modelMatrixUniform;
	video::Uniform normalMatrixUniform;
	video::Uniform viewProjectionMatrixUniform;
	video::Attribute positionAttribute;
	video::Attribute normalAttribute;
	video::Attribute uvAttribute;
};

} // util
} // flat

#endif // FLAT_UTIL_RENDERSETTINGS_H



