#include "csvmatrix.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>


// is it row major or column major?
// it is row major
// which means fix the row index to be 0, read all elements from the vector
std::string CSVMatrix::GetColumnNamesAsString() const
{
    if(m_rows > 0)
    {
        std::string column_names;

        for(const auto& element : m_matrix.at(0))
        {
            // TODO: something better than this
            const std::string column_name(element.GetString());
            std::string tmp(column_name);
            const std::string nl("\n");
            column_names += tmp + nl;
        }

        return column_names;
    }
    else
    {
        return std::string("Error: Number of rows is 0");
    }
}