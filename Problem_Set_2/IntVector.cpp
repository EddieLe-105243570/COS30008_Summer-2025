// Problem Set 2, 2022
// IntVector.cpp - Implementation

#include "IntVector.h"
#include <stdexcept>

// Constructor: copy argument array
IntVector::IntVector(const int aArrayOfIntegers[], size_t aNumberOfElements)
{
	fNumberOfElements = aNumberOfElements;
	fElements = new int[fNumberOfElements];

	for (size_t i = 0; i < fNumberOfElements; i++)
	{
		fElements[i] = aArrayOfIntegers[i];
	}
}

// Destructor: release memory
IntVector::~IntVector()
{
	delete[] fElements;
}

// size getter
size_t IntVector::size() const
{
	return fNumberOfElements;
}

// indexer - returns element at aIndex with range checking
const int IntVector::operator[](size_t aIndex) const
{
	if (aIndex >= fNumberOfElements)
	{
		throw std::out_of_range("Index out of range");
	}

	return fElements[aIndex];
}

// element getter - implemented using operator[]
const int IntVector::get(size_t aIndex) const
{
	return (*this)[aIndex];
}

// swap two elements within the vector
void IntVector::swap(size_t aSourceIndex, size_t aTargetIndex)
{
	if (aSourceIndex >= fNumberOfElements || aTargetIndex >= fNumberOfElements)
	{
		throw std::out_of_range("Index out of range");
	}

	int temp = fElements[aSourceIndex];
	fElements[aSourceIndex] = fElements[aTargetIndex];
	fElements[aTargetIndex] = temp;
}