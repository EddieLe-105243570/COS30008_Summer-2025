// iVigenereStream.cpp
#include "iVigenereStream.h"
#include <cstdint>

using namespace std;

iVigenereStream::iVigenereStream(Cipher aCipher, const std::string &aKeyword, const char *aFileName)
    : fCipherProvider(aKeyword), fCipher(aCipher)
{
    if (aFileName)
        open(aFileName);
}

iVigenereStream::~iVigenereStream()
{
    close();
}

void iVigenereStream::open(const char *aFileName)
{
    fIStream.open(aFileName, ios::in | ios::binary);
}

void iVigenereStream::close()
{
    fIStream.close();
}

void iVigenereStream::reset()
{
    fCipherProvider.reset();
    seekstart();
}

bool iVigenereStream::good() const
{
    return fIStream.good();
}

bool iVigenereStream::is_open() const
{
    return fIStream.is_open();
}

bool iVigenereStream::eof() const
{
    return fIStream.eof();
}

uint64_t iVigenereStream::position()
{
    return fIStream.tellg();
}

void iVigenereStream::seekstart()
{
    fIStream.clear();
    fIStream.seekg(0, ios_base::beg);
}

iVigenereStream &iVigenereStream::operator>>(char &aCharacter)
{
    char lChar;
    if (fIStream.get(lChar))
    {
        aCharacter = fCipher(fCipherProvider, lChar);
    }
    else
    {
        fIStream.clear();
    }
    return *this;
}