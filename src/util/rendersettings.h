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
	video::Uniform<video::Texture> textureUniform;
	video::Uniform<bool>           textureGivenUniform;
	video::Uniform<video::Color>   colorUniform;
	video::Uniform<video::Color>   secondaryColorUniform;
	video::Uniform<video::Texture> bumpMapUniform;
	video::Uniform<Matrix4>        modelMatrixUniform;
	video::Uniform<Matrix4>        normalMatrixUniform;
	video::Uniform<Matrix4>        viewProjectionMatrixUniform;

	video::Attribute positionAttribute;
	video::Attribute normalAttribute;
	video::Attribute uvAttribute;
	video::Attribute colorAttribute;
};

} // util
} // flat

#endif // FLAT_UTIL_RENDERSETTINGS_H



