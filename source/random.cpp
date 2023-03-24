#include "pch.h"
#include "random.h"

namespace kazin
{
bool Random::Probability(const float num)
{
	return (num > 1.f ? true : (num < 0.f ? false : std::bernoulli_distribution(num)(engine_)));
}
}
