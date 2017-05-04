#ifndef FLAT_DEBUG_HELPERS_H
#define FLAT_DEBUG_HELPERS_H

#include "assert.h"

#ifdef FLAT_VISUAL_STUDIO

#define FLAT_PRAGMA(x) __pragma(x)

#define FLAT_OPTIMIZE_OFF() FLAT_PRAGMA(optimize("", off))
#define FLAT_OPTIMIZE_ON()  FLAT_PRAGMA(optimize("", on))

#else
#error not implemented for this compiler
#endif

#endif // FLAT_DEBUG_HELPERS_H


