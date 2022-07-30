#include"Random.h"

namespace itbs
{
	namespace Math
	{
		std::mt19937 Random::mt(std::random_device{}());

		float Random::Range(const float min, const float max)
		{
			std::uniform_real_distribution<> dist(min, max);

			return static_cast<float>(dist(mt));
		}

		bool Random::Probability(float probability)
		{
			probability = std::fmaxf(0.0f, probability);
			probability = std::fminf(1.0f, probability);

			if (probability == 0.0f)
			{
				return false;
			}

			return Range(0.0f, 1.0f) <= probability;
		}
	}
}