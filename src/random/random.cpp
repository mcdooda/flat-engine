#include <ctime>
#include <cstdlib>

#include "random/random.h"

namespace flat
{
namespace random
{

Random::Random()
{
	setSeed(static_cast<int>(time(nullptr)));
}

Random::~Random()
{
	
}

int Random::nextInt(int min, int max)
{
	return min + rand() % (max - min + 1);
}

float Random::nextFloat(float min, float max)
{
	return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}

void Random::resetSeed()
{
	setSeed(m_seed);
}

void Random::setSeed(int seed)
{
	m_seed = seed;
	srand(seed);
}

} // random
} // flat


