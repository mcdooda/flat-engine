#ifndef FLAT_DEBUG_ASSERT_H
#define FLAT_DEBUG_ASSERT_H

#ifdef FLAT_DEBUG

#include <cstdio>
#include <cassert>
#include <csignal>

#ifdef _MSC_VER
#define FLAT_VISUAL_STUDIO
#endif

#ifdef FLAT_VISUAL_STUDIO
#define FLAT_BREAK() __debugbreak()
#define FLAT_PRETTY_FUNCTION __FUNCSIG__
#else
#define FLAT_BREAK() raise(SIGTRAP)
#define FLAT_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif


#define FLAT_ASSERT_MSG(cond, format, ...) \
	if (!(cond)) \
	{ \
		fprintf(stderr, "assertion failed: " #cond " ; " format "\n" \
			"in file " __FILE__ ":%d\n" \
			"in function %s\n", \
			##__VA_ARGS__, __LINE__, FLAT_PRETTY_FUNCTION); \
		FLAT_BREAK(); \
	}

#define FLAT_ASSERT(cond) \
	if (!(cond)) \
	{ \
		fprintf(stderr, "assertion failed: " #cond "\n" \
			"in file " __FILE__ ":%d\n" \
			"in function %s\n", \
			__LINE__, FLAT_PRETTY_FUNCTION); \
		FLAT_BREAK(); \
	}

#define FLAT_DEBUG_ONLY(code) \
	code

#else // FLAT_DEBUG

#define FLAT_BREAK() \
	{}

#define FLAT_ASSERT_MSG(cond, format, ...) \
	{}

#define FLAT_ASSERT(cond) \
	{}

#define FLAT_DEBUG_ONLY(code)

#endif // FLAT_DEBUG

#define FLAT_CT_PRINT(X) namespace { template <int S> struct CtPrint; CtPrint<X> ctprint; }

#endif // FLAT_DEBUG_ASSERT_H



