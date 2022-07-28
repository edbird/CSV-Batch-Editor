
#include "csvmatrix.h"


CSVMatrix CSVMatrix::ColumnCopy(const std::string& from_column_name, const std::string& to_column_name)
{
    matrix_row_t::size_type from_column_index(GetColumnIndex(from_column_name));
    matrix_row_t::size_type to_column_index(GetColumnIndex(to_column_name));

    if(from_column_index <= to_column_index)
    {
        return ColumnCopy(from_column_index, to_column_index - from_column_index + 1);
    }
    else
    {
        std::string e
        {
            std::string("Error: Cannot copy from column ")
                + from_column_name
                + std::string(" to column")
                + to_column_name
                + std::string( " because ")
                + from_column_name
                + std::string(" is located after ")
                + to_column_name
        };
        throw std::runtime_error(e);
    }
}

CSVMatrix CSVMatrix::ColumnCopy(const matrix_row_t::size_type start_column, const matrix_row_t::size_type count)
{
    CSVMatrix tmp(*this);

    for(const auto ix_col{start_column + count}; ix_col + 1 < m_columns; )
    {
        // Each time this function is called the number of columns (m_columns)
        // is decreased by 1
        DeleteColumn(ix_col);
    }

    for(; m_columns > count; )
    {
        DeleteColumn(0);
    }

    return tmp;
}

// should be inclusive of the start and end index
CSVMatrix CSVMatrix::ColumnCopy2(matrix_row_t::size_type start_column, matrix_row_t::size_type end_column)
{
    CSVMatrix tmp(*this);

    for(auto ix_col{end_column + 1}; ix_col < m_columns; ++ ix_col)
    {

    }

    return tmp;
}

// TODO: implement this square version which permits the number of rows and columns to be changed
// simultaniously
/*
CSVMatrix CSVMatrix::BlockCopy(matrix_row_t::size_type start_column, matrix_t::size_type end_column,
                    matrix_t::size_type start_row, matrix_t end_row)
{
    // there will be a more efficient way to do this which doesn't involve copying everything, 
    // but it would require more complex logic to handle setting the number of rows and columns
    // in advance, in addition to the m_columns_set variable

    CSVMatrix tmp(*this);

    for(const auto ix_row{end_row}; ix_row < m_rows; ++ ix_row)
    {
        // does m_rows change?
    }
}
*/