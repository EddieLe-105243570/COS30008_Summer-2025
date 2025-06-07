#include "BernsteinBasisPolynomial.h"
#include <cmath>

BernsteinBasisPolynomial::BernsteinBasisPolynomial(unsigned int aV, unsigned int aN)
	: fFactor(aN, aV)  // Combination(n, k) đề là b(v, n)
{
}

double BernsteinBasisPolynomial::operator()(double aX) const
{
	unsigned long long comb = fFactor(); // C(n, v)
	unsigned int v = fFactor.getK();
	unsigned int n = fFactor.getN();

	return comb * pow(aX, v) * pow(1 - aX, n - v);
}