#ifndef FLAT_RANDOM_H
#define FLAT_RANDOM_H

namespace flat
{
namespace random
{

class Random
{
	public:
		Random();
		~Random();
		
		int nextInt(int min, int max);
		float nextFloat(float min, float max);
		void resetSeed();
		void setSeed(int seed);
		
	private:
		int m_seed;

};

} // random
} // flat

#endif // FLAT_RANDOM_H


