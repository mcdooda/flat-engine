#ifndef FLAT_RENDER_PROGRAMSETTINGS_H
#define FLAT_RENDER_PROGRAMSETTINGS_H

#include "render/rendersettings.h"

#include "video/program.h"

namespace flat
{
namespace render
{

struct ProgramSettings
{
	video::Program program;
	RenderSettings settings;
};

} // render
} // flat

#endif // FLAT_RENDER_PROGRAMSETTINGS_H


