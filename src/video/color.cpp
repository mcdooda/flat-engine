#include "video/color.h"

namespace flat
{
namespace video
{

Color Color::WHITE;
Color Color::BLACK = Color(0.0f);
Color Color::RED   = Color(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
Color Color::BLUE  = Color(0.0f, 0.0f, 1.0f, 1.0f);

} // video
} // flat


