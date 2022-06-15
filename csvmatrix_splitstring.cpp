#include "csvmatrix.h"


#include <string>
#include <vector>
#include <iostream>




std::vector<std::string> CSVMatrix::SplitStringByToken(std::string s, const std::string& token)
{
    std::vector<std::string> ret;

    for(;;)
    {
        std::string::size_type pos(s.find(token));

        if(pos != std::string::npos)
        {
            // TODO: need to check pos > 0 for all calls to substr
            if(pos > 0)
            {
                std::string subs(s.substr(0, pos));
                ret.push_back(subs);

                if(pos + 1 < s.size())
                {
                    // not the last character in the string
                    s = s.substr(pos + 1);
                }
                else
                {
                    // last character in string
                    // and delimiter was found
                    ret.push_back(std::string());
                    break; // TODO: no idea if this logic is correct here!
                }
            }
            else // no longer need this?
            {
                ret.push_back(std::string());

                if(pos + 1 < s.size())
                {
                    // not the last character in the string
                    s = s.substr(pos + 1);
                }
                else
                {
                    // last character in string
                    // and delimiter NOT found
                    break; // TODO: no idea if this logic is correct here!
                }
            }
        }
        else
        {
            // delimiter not found, add remaining string to vector
            ret.push_back(s);
            break;
        }
    }

    /*
    for(const auto& s : ret)
    {
        std::cout << "split string: " << s << std::endl;
    }
    */

    return ret;
}



std::vector<std::string> CSVMatrix::SplitStringByTokenQuoted(std::string s, const std::string& token, const std::string& token_quote)
{
    std::vector<std::string> ret;

    for(;;)
    {
        std::string::size_type pos(s.find(token));
        std::string::size_type pos_quote(s.find(token_quote));

        if(pos != std::string::npos)
        {
            // A (logic block)

            // found a quote, and it occurs before the next comma
            if(pos_quote < pos)
            {
                // need to find another quote
                std::string::size_type pos_quote_2{s.find(token_quote, pos_quote + 1)};
                    // should it be quote + 1 here? can this overflow the length of the string? is that a problem?

                if(pos_quote_2 != std::string::npos)
                {
                    // find the next comma following the second quote
                    pos = s.find(token, pos_quote_2 + 1);

                    // might be able to move this logic block, becuase it is the same
                    // as the logic block which encloses this block (marked A)
                    if(pos == std::string::npos)
                    {
                        // delimiter not found, add remaining string to vector
                        ret.push_back(s);
                        break;
                    }
                    // else
                    // continue with existing logic and end of A
                }
            }
            // end of A

            ////////////////////////////////////////////////////////////////////
            // "Normal" execution block where next string, delimited by comma //
            // does NOT contain a quote character                             //
            ////////////////////////////////////////////////////////////////////

            // TODO: need to check pos > 0 for all calls to substr
            if(pos > 0)
            {
                std::string subs(s.substr(0, pos));
                ret.push_back(subs);

                if(pos + 1 < s.size())
                {
                    // not the last character in the string
                    s = s.substr(pos + 1);
                }
                else
                {
                    // last character in string
                    // and delimiter was found
                    ret.push_back(std::string());
                    break; // TODO: no idea if this logic is correct here!
                }
            }
            else // no longer need this?
            {
                ret.push_back(std::string());

                if(pos + 1 < s.size())
                {
                    // not the last character in the string
                    s = s.substr(pos + 1);
                }
                else
                {
                    // last character in string
                    // and delimiter NOT found
                    break; // TODO: no idea if this logic is correct here!
                }
            }
        }
        else
        {
            // delimiter not found, add remaining string to vector
            ret.push_back(s);
            break;
        }
    }

    /*
    for(const auto& s : ret)
    {
        std::cout << "split string: " << s << std::endl;
    }
    */

    return ret;
}