#ifndef FLAT_UTIL_VARIADICHELPERS_H
#define FLAT_UTIL_VARIADICHELPERS_H

#define FLAT_VARIADIC_EXPAND(X) \
	static_cast<void>(std::initializer_list<int>{ (static_cast<void>(X), 0)... })

#endif // FLAT_UTIL_VARIADICHELPERS_H

