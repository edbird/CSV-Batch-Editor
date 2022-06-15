
#include "csvmatrix.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>



void CSVMatrix::SetColumn(const std::string& column_name, const std::string& value)
{
    matrix_row_t::size_type column_index(GetColumnIndex(column_name));
    SetColumn(column_index, value, true);
}


// TODO: first_row_is_header_row not used?
void CSVMatrix::SetColumn(const matrix_t::size_type column_index, const std::string& value, const bool first_row_is_header_row)
{
    if(column_index < m_columns)
    {
        for(matrix_row_t::size_type index{0}; index < m_rows; ++ index)
        {
            if(first_row_is_header_row && index == 0)
            {
                continue;
            }

            m_matrix.at(index).at(column_index).SetString(value);
        }
    }
    else
    {
        std::string e
        {
            std::string("Error: Cannot set column values for column with index ")
                + std::to_string(column_index)
                + std::string(". Number of columns is ")
                + std::to_string(m_columns)
        };
        throw std::runtime_error(e);
    }
}