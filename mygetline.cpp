#include "mygetline.h"


#include <iostream>
#include <string>
#include <algorithm>


// TODO: move to another file
void myfunctions::EraseCRLF(std::string &s)
{
    s.erase(std::remove(s.begin(), s.end(), '\r' ), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\n' ), s.end());
}


template<class CharT, class Traits, class Allocator>
std::basic_istream<CharT, Traits>& myfunctions::GetLine(
    std::basic_istream<CharT,Traits>& input,
    std::basic_string<CharT,Traits,Allocator>& str)
{
    auto& ret = std::getline(input, str);
    myfunctions::EraseCRLF(str);
    return ret;
}