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


