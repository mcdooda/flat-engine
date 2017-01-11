#ifndef FLAT_MISC_MATH_H
#define FLAT_MISC_MATH_H

namespace flat
{

template <class T>
inline T normalize(const T& vector)
{
	if (glm::length2(vector) == 0)
		return vector;

	return glm::normalize(vector);
}

constexpr float PI = glm::pi<float>();
constexpr float PI2 = PI * 2.f;

constexpr float SQRT2 = 1.41421356237f;

template <typename T>
int8_t sign(T number)
{
	return (T(0) < number) - (number < T(0));
}

} // flat

#endif // FLAT_MISC_MATH_H



