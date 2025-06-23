// Vigenere.cpp
#include "Vigenere.h"
#include <cctype>

Vigenere::Vigenere(const std::string &aKeyword)
    : fKeyword(aKeyword), fKeywordProvider(aKeyword)
{
    initializeTable();
}

void Vigenere::initializeTable()
{
    for (char row = 0; row < CHARACTERS; row++)
    {
        char lChar = 'A' + row;
        for (char column = 0; column < CHARACTERS; column++)
        {
            fMappingTable[row][column] = lChar;
            if (++lChar > 'Z')
                lChar = 'A';
        }
    }
}

std::string Vigenere::getCurrentKeyword()
{
    std::string result;
    Vigenere temp(*this);
    for (size_t i = 0; i < fKeyword.length(); ++i)
    {
        result += *temp.fKeywordProvider;
        temp.fKeywordProvider << result.back();
    }
    return result;
}

void Vigenere::reset()
{
    fKeywordProvider.initialize(fKeyword);
}

char Vigenere::encode(char aCharacter)
{
    if (!std::isalpha(aCharacter))
        return aCharacter;
    bool isLower = std::islower(aCharacter);
    char keyChar = *fKeywordProvider;
    char encoded = fMappingTable[keyChar - 'A'][std::toupper(aCharacter) - 'A'];
    fKeywordProvider << aCharacter;
    return isLower ? std::tolower(encoded) : encoded;
}

char Vigenere::decode(char aCharacter)
{
    if (!std::isalpha(aCharacter))
        return aCharacter;
    bool isLower = std::islower(aCharacter);
    char keyChar = *fKeywordProvider;
    int row = keyChar - 'A';
    int col = 0;
    while (fMappingTable[row][col] != std::toupper(aCharacter))
        ++col;
    fKeywordProvider << (col + 'A');
    return isLower ? std::tolower(col + 'A') : col + 'A';
}
