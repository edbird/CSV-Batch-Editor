#ifndef MYGETLINE_H
#define MYGETLINE_H


#include <iostream>
#include <string>


namespace myfunctions
{

    
    // TODO: move to another file
    void EraseCRLF(std::string &s);


    template<class CharT, class Traits, class Allocator>
    std::basic_istream<CharT, Traits>& GetLine(
        std::basic_istream<CharT,Traits>& input,
        std::basic_string<CharT,Traits,Allocator>& str);

}

#endif // MYGETLINE_H