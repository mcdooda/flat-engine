#ifndef FLAT_MISC_MATH_H
#define FLAT_MISC_MATH_H

namespace flat
{

constexpr float PI = 3.14159265358979323846264338327950288f;
constexpr float PI_2 = PI * 2.f;

constexpr float PI_DIV_2 = PI / 2.f;
constexpr float PI_DIV_4 = PI / 4.f;

constexpr float SQRT_2 = 1.41421356237f;
constexpr float SQRT_2_DIV_2 = SQRT_2 / 2.f;

constexpr float SQRT_2_INV = 1.f / SQRT_2;

constexpr float EPSILON = 0.00001f;

template <typename T>
int8_t sign(T number)
{
	return (T(0) < number) - (number < T(0));
}

inline float angle_clamp_pi(float angle)
{
	angle = fmodf(angle, flat::PI_2);
	if (angle < -flat::PI)
	{
		angle += flat::PI_2;
	}
	else if (angle > flat::PI)
	{
		angle -= flat::PI_2;
	}
	return angle;
}

inline float angle_clamp_0_2pi(float angle)
{
	angle = fmodf(angle, flat::PI_2);

	if (angle < 0.f)
		angle += flat::PI_2;

	return angle;
}

template <class T>
inline bool isInRange(T value, T min, T max)
{
	return min <= value && value <= max;
}

template <class T>
inline bool areValuesClose(T value, T other, T epsilon)
{
	return isInRange(value, other - epsilon, other + epsilon);
}

} // flat

#endif // FLAT_MISC_MATH_H



