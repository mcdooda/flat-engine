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
	video::Uniform modelMatrixUniform;
	video::Uniform vpMatrixUniform;
	video::Attribute positionAttribute;
	video::Attribute uvAttribute;
};

} // util
} // flat

#endif // FLAT_UTIL_RENDERSETTINGS_H



