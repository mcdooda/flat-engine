#ifndef FLAT_MISC_STD_H
#define FLAT_MISC_STD_H

namespace flat
{

// std::size coming in c++17
template <class T, std::size_t N>
constexpr std::size_t size(const T (&array)[N]) noexcept
{
	return N;
}

} // flat

#endif // FLAT_MISC_STD_H



