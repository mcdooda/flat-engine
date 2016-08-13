#ifndef FLAT_DEBUG_ASSERT_H
#define FLAT_DEBUG_ASSERT_H

#ifdef FLAT_DEBUG

#include <cstdio>
#include <cassert>
#include <csignal>

#define FLAT_BREAK() \
	raise(SIGTRAP)

#define FLAT_ASSERT_MSG(cond, format, ...) \
	if (!(cond)) \
	{ \
		fprintf(stderr, "assertion failed: " #cond " ; " format "\n" \
			"in file " __FILE__ ":%d\n" \
			"in function %s\n", \
			##__VA_ARGS__, __LINE__, __PRETTY_FUNCTION__); \
		FLAT_BREAK(); \
	}

#define FLAT_ASSERT(cond) \
	if (!(cond)) \
	{ \
		fprintf(stderr, "assertion failed: " #cond "\n" \
			"in file " __FILE__ ":%d\n" \
			"in function %s\n", \
			__LINE__, __PRETTY_FUNCTION__); \
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



