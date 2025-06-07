#include "Combination.h"

Combination::Combination(size_t aN, size_t aK)
	: fN(aN), fK(aK)
{
}

size_t Combination::getN() const
{
	return fN;
}

size_t Combination::getK() const
{
	return fK;
}

unsigned long long Combination::operator()() const
{
	if (fK > fN)
		return 0;

	if (fK == 0 || fK == fN)
		return 1;

	// dùng công thức lặp
	unsigned long long result = 1;

	for (size_t i = 1; i <= fK; ++i)
	{
		result *= (fN - (i - 1));
		result /= i;
	}

	return result;
}