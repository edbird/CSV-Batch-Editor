#include "csvmatrix.h"



void CSVMatrix::AddColumnBefore(const std::string& column_name,
                        const std::string& new_column_name)
{
    std::cerr << "AddColumnBefore not implemented" << std::endl;
}

// TODO: version of this with a value
// And command "add column name after existing_name with value v"
void CSVMatrix::AddColumnAfter(const std::string& column_name,
                    const std::string& new_column_name)
{
    matrix_row_t::size_type column_index(GetColumnIndex(column_name));
    AddColumnAfter(column_index, new_column_name, true);
    
    // TODO: first_row_is_header_row not used?
}

// TODO: first_row_is_header_row not used/implemented
void CSVMatrix::AddColumnAfter(matrix_t::size_type column_index,
                    const std::string& column_name,
                    const bool first_row_is_header_row)
{
    if(column_index < m_columns)
    {
        for(matrix_t::size_type index{0}; index < m_rows; ++ index)
        {
            CSVMatrixElement element("", false);
            if(first_row_is_header_row && index == 0)
            {
                //std::cout << "set column name: " << column_name << std::endl;
                element.SetString(column_name);
                element.SetHeaderRow(true);
            }

            auto &row{m_matrix.at(index)};
            row.insert(row.begin() + column_index, element);
        }

        ++ m_columns;
    }
    else
    {
        std::string e
        {
            std::string("Error: Attempt to add column after column index ")
                + std::to_string(column_index)
                + std::string(" which is out of range. Number of columns is ")
                + std::to_string(m_columns)
        };
        throw std::runtime_error(e);
    }
}