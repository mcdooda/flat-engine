#ifndef ENGINE_RANDOM_H
#define ENGINE_RANDOM_H

namespace engine
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
} // engine

#endif // ENGINE_RANDOM_H


