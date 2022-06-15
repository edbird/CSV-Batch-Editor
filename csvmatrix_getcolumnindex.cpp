#include "csvmatrix.h"



bool CSVMatrix::ColumnExists(const std::string& column_name) const
{
    if(m_matrix.size() > 0)
    {
        const matrix_row_t &first_row(*m_matrix.cbegin());
    
        auto predicate = [&column_name](const CSVMatrixElement& element)
        {
            return element.GetString() == column_name;
        };

        auto it(std::find_if(first_row.cbegin(), first_row.cend(), predicate));
        
        if(it == first_row.cend())
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}


matrix_row_t::size_type CSVMatrix::GetColumnIndex(const std::string& column_name) const
{
    if(m_matrix.size() > 0)
    {
        const matrix_row_t &first_row(*m_matrix.cbegin());
    
        auto predicate = [&column_name](const CSVMatrixElement& element)
        {
            return element.GetString() == column_name;
        };

        auto it(std::find_if(first_row.cbegin(), first_row.cend(), predicate));
        
        if(it == first_row.cend())
        {
            std::string e
            {
                std::string("Error: Could not find column with name ")
                    + column_name
            };

            /*
            std::cout << "list of columns" << std::endl;
            for(auto &col : m_matrix.at(0))
            {
                std::cout << col.GetString() << std::endl;
            }
            */

            throw std::runtime_error(e);
        }

        matrix_row_t::size_type column_index(std::distance(first_row.cbegin(), it));
        return column_index;
    }
    else
    {
        std::string e
        {
            std::string("Error: Cannot find column with name ")
                + column_name
                + std::string(" because number of rows is 0")
        };
        throw std::runtime_error(e);
    }
}