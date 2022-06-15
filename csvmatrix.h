#ifndef CSVMATRIX_H
#define CSVMATRIX_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>


// class to store and manipulate CSV data
class CSVMatrix
{
    
    typedef std::vector<CSVMatrixElement> matrix_row_t;
    typedef std::vector<matrix_row_t> matrix_t;

    public:

    CSVMatrix()
        : m_columns(0)
        , m_rows(0)
        , m_columns_set(false)
    {

    }

    virtual
    ~CSVMatrix()
    {

    }

    std::vector<std::string> SplitStringByTokenQuoted(std::string s, const std::string& token, const std::string& token_quote);

    std::vector<std::string> SplitStringByToken(std::string s, const std::string& token);

    void ReadFromFile(const std::string& filename, const bool first_row_is_header_row);

    void WriteToFile(const std::string& filename) const;

    void WriteToFile() const;

    void AddColumnBefore(const std::string& column_name,
                         const std::string& new_column_name);

    // TODO: version of this with a value
    // And command "add column name after existing_name with value v"
    void AddColumnAfter(const std::string& column_name,
                        const std::string& new_column_name);

    // TODO: first_row_is_header_row not used/implemented
    void AddColumnAfter(matrix_t::size_type column_index,
                        const std::string& column_name,
                        const bool first_row_is_header_row);

    void ReorderColumnAfter(const std::string& reference_column_name,
                            const std::string& column_to_move_name);

    matrix_row_t::size_type GetColumnIndex(const std::string& column_name) const;

    bool ColumnExists(const std::string& column_name) const;

    void SetColumn(const std::string& column_name, const std::string& value)
    {
        matrix_row_t::size_type column_index(GetColumnIndex(column_name));
        SetColumn(column_index, value, true);
    }

    // TODO: first_row_is_header_row not used?
    void SetColumn(const matrix_t::size_type column_index, const std::string& value, const bool first_row_is_header_row)
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

    void DeleteColumn(const std::string& column_name)
    {
        matrix_row_t::size_type column_index(GetColumnIndex(column_name));
        DeleteColumn(column_index);
    }

    // is it row major or column major?
    void DeleteColumn(const matrix_t::size_type column_index)
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

    // is it row major or column major?
    // it is row major
    // which means fix the row index to be 0, read all elements from the vector
    std::string GetColumnNamesAsString() const
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

    private:

    // row major: row index comes first
    // which means row insertion / deletion is more efficient
    // than column insertion / deletion
    matrix_t m_matrix;

    // original filename used to load data
    std::string m_filename;

    // number of rows and columns
    std::size_t m_columns;
    std::size_t m_rows;

    // if the number of columns has been set, this value is true
    // this will be set and used when reading in data
    // it is set by the number of elements in the first row which
    // is read from file
    // TODO: not all csv files are formatted like this, fix
    // for cases where the number of columns is variable (find 
    // maximum)
    bool m_columns_set;

    // TODO enable/disable/force quote each field
    // options:
    // force quote all fields
    // quote only fields which were quoted when reading (no change)
    // force do not quote fields
    // then: other combinations for empty fields!
    
    // TODO: line endings
    // options:
    // force CRLF
    // force LF
    // use system default
    // do not change (use values from reading file)

};

#endif // CSVMATRIX_H