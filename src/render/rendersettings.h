#ifndef FLAT_RENDER_RENDERSETTINGS_H
#define FLAT_RENDER_RENDERSETTINGS_H

#include "../video/uniform.h"
#include "../video/attribute.h"

namespace flat
{
namespace render
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
	video::Uniform<bool>           vertexColorGivenUniform;

	video::Attribute positionAttribute;
	video::Attribute normalAttribute;
	video::Attribute uvAttribute;
	video::Attribute colorAttribute;
	video::Attribute depthAttribute;

	std::uint32_t getHash() const;

	RenderSettings() :
		positionAttribute(-1),
		normalAttribute(-1),
		uvAttribute(-1),
		colorAttribute(-1),
		depthAttribute(-1)
	{

	}
};

} // render
} // flat

#endif // FLAT_RENDER_RENDERSETTINGS_H



