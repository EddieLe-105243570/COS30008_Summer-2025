// VigenereForwardIterator.cpp
#include "VigenereForwardIterator.h"

VigenereForwardIterator::VigenereForwardIterator(iVigenereStream &aIStream)
    : fIStream(aIStream), fEOF(false)
{
    ++(*this);
}

char VigenereForwardIterator::operator*() const
{
    return fCurrentChar;
}

VigenereForwardIterator &VigenereForwardIterator::operator++()
{
    if (fIStream >> fCurrentChar)
    {
        fEOF = false;
    }
    else
    {
        fEOF = true;
    }
    return *this;
}

VigenereForwardIterator VigenereForwardIterator::operator++(int)
{
    VigenereForwardIterator temp = *this;
    ++(*this);
    return temp;
}

bool VigenereForwardIterator::operator==(const VigenereForwardIterator &aOther) const
{
    return (&fIStream == &aOther.fIStream) && (fEOF == aOther.fEOF);
}

bool VigenereForwardIterator::operator!=(const VigenereForwardIterator &aOther) const
{
    return !(*this == aOther);
}

VigenereForwardIterator VigenereForwardIterator::begin() const
{
    VigenereForwardIterator it(fIStream);
    it.fIStream.reset();
    return it;
}

VigenereForwardIterator VigenereForwardIterator::end() const
{
    VigenereForwardIterator it(fIStream);
    it.fEOF = true;
    return it;
}
