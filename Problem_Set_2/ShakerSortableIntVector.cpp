// Problem Set 2, 2022
// ShakerSortableIntVector.cpp - Implementation

#include "ShakerSortableIntVector.h"

// Constructor: uses member initializer to call parent constructor
ShakerSortableIntVector::ShakerSortableIntVector(const int aArrayOfIntegers[], size_t aNumberOfElements)
	: SortableIntVector(aArrayOfIntegers, aNumberOfElements)
{
	// Parent constructor handles all initialization
}

// Cocktail Shaker Sort implementation
// This implementation sorts in DECREASING order using the default lambda
void ShakerSortableIntVector::sort(Comparable aOrderFunction)
{
	size_t n = size();

	if (n <= 1)
		return;

	size_t left = 0;
	size_t right = n - 1;

	while (left < right)
	{
		// Forward pass (left to right)
		// Move the largest element to the right
		for (size_t i = left; i < right; i++)
		{
			// For decreasing order: if left element is smaller than right element, swap
			// Since aOrderFunction returns true for aLeft <= aRight,
			// we swap when the function returns true (meaning left <= right)
			if (aOrderFunction((*this)[i], (*this)[i + 1]))
			{
				swap(i, i + 1);
			}
		}
		right--;

		// Backward pass (right to left)
		// Move the smallest element to the left
		for (size_t i = right; i > left; i--)
		{
			// For decreasing order: if left element is smaller than right element, swap
			if (aOrderFunction((*this)[i - 1], (*this)[i]))
			{
				swap(i - 1, i);
			}
		}
		left++;
	}
}