// Problem Set 2, 2022
// SortableIntVector.cpp - Implementation

#include "SortableIntVector.h"

// Constructor: uses member initializer to call parent constructor
SortableIntVector::SortableIntVector(const int aArrayOfIntegers[], size_t aNumberOfElements)
	: IntVector(aArrayOfIntegers, aNumberOfElements)
{
	// Parent constructor handles all initialization
}

// Bubble Sort implementation
void SortableIntVector::sort(Comparable aOrderFunction)
{
	size_t n = size();

	// Bubble Sort algorithm
	for (size_t i = 0; i < n - 1; i++)
	{
		for (size_t j = 0; j < n - 1 - i; j++)
		{
			// Compare adjacent elements using the provided comparison function
			// If left element should NOT come before right element, swap them
			if (!aOrderFunction((*this)[j], (*this)[j + 1]))
			{
				swap(j, j + 1);
			}
		}
	}
}