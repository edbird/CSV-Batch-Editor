
#include "csvmatrix.h"


void CSVMatrix::DeleteColumn(const std::string& column_name)
{
    matrix_row_t::size_type column_index(GetColumnIndex(column_name));
    DeleteColumn(column_index);
}

// is it row major or column major?
void CSVMatrix::DeleteColumn(const matrix_t::size_type column_index)
{
    if(column_index < m_columns)
    {
        for(matrix_t::size_type index{0}; index < m_rows; ++ index)
        {
            auto &row{m_matrix.at(index)};
            row.erase(row.begin() + column_index);
        }

        -- m_columns;
    }
    else
    {
        std::string e
        {
            std::string("Error: Attempt to delete column after column index ")
                + std::to_string(column_index)
                + std::string(" which is out of range. Number of columns is ")
                + std::to_string(m_columns)
        };
        throw std::runtime_error(e);
    }
}