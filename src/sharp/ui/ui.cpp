#include "ui.h"
#include "lua/ui.h"
#include "../../flat.h"

namespace flat
{
namespace sharp
{
namespace ui
{

Ui::Ui(Flat& flat) :
	factory(flat)
{
	root = factory.makeRoot();
	lua::setRootWidget(flat.lua->state, root.get());
}

} // ui
} // sharp
} // flat