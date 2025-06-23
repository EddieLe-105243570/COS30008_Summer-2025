// KeyProvider.cpp
#include "KeyProvider.h"
#include <cstring>

KeyProvider::KeyProvider(const std::string& aKeyword) : fKeyword(nullptr), fSize(0), fIndex(0) {
    initialize(aKeyword);
}

KeyProvider::~KeyProvider() {
    delete[] fKeyword;
}

void KeyProvider::initialize(const std::string& aKeyword) {
    delete[] fKeyword;
    fSize = aKeyword.length();
    fKeyword = new char[fSize];
    for (size_t i = 0; i < fSize; ++i) {
        fKeyword[i] = std::toupper(aKeyword[i]);
    }
    fIndex = 0;
}

char KeyProvider::operator*() const {
    return fKeyword[fIndex];
}

KeyProvider& KeyProvider::operator<<(char aKeyCharacter) {
    fKeyword[fIndex] = std::toupper(aKeyCharacter);
    fIndex = (fIndex + 1) % fSize;
    return *this;
}
