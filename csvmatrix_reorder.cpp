#include "csvmatrix.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>



void CSVMatrix::ReorderColumnAfter(const std::string& reference_column_name,
                        const std::string& column_to_move_name)
{
    matrix_row_t::size_type reference_column_index{GetColumnIndex(reference_column_name)};
    matrix_row_t::size_type column_to_move_index{GetColumnIndex(column_to_move_name)};

    if(reference_column_index != column_to_move_index)
    {
        if(reference_column_index < m_columns && column_to_move_index < m_columns)
        {
            // TODO: steps:
            // extract the existing column (copy it) to a new CSVMatrix object
            // delete the existing column
            // insert a new column into the CSVMatrix with values referenced from the new CSVMatrix object
            // insert entire object, not a column indexed from that object
        }
    }
    else
    {
        // TODO: exception
    }
}