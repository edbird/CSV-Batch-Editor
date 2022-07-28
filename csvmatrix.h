#ifndef CSVMATRIX_H
#define CSVMATRIX_H


#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>


#include "csvmatrixelement.h"


// class to store and manipulate CSV data
class CSVMatrix
{
    
    typedef std::vector<CSVMatrixElement> matrix_row_t;
    typedef std::vector<matrix_row_t> matrix_t;

    public:

    friend void swap(CSVMatrix& l, CSVMatrix& r)
    {
        using std::swap;

        swap(l.m_matrix, r.m_matrix);
        swap(l.m_filename, r.m_filename);
        swap(l.m_columns, r.m_columns);
        swap(l.m_rows, r.m_rows);
        swap(l.m_columns_set, r.m_columns_set);
    }

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

    CSVMatrix(const CSVMatrix& other)
        : m_matrix{other.m_matrix}
        , m_filename{other.m_filename}
        , m_columns{other.m_columns}
        , m_rows{other.m_rows}
        , m_columns_set{other.m_columns_set}
    {

    }

    CSVMatrix operator=(CSVMatrix other)
    {
        swap(*this, other);

        return *this;
    }

    CSVMatrix(CSVMatrix&& other) noexcept
        : CSVMatrix()
    {
        swap(*this, other);
    }


    CSVMatrix ColumnCopy(const std::string& from_column_name, const std::string& to_column_name);

    CSVMatrix ColumnCopy(const matrix_row_t::size_type start_column, const matrix_row_t::size_type count);

    // should be inclusive of the start and end index
    CSVMatrix ColumnCopy2(matrix_row_t::size_type start_column, matrix_row_t::size_type end_column);

    // TODO: implement this square version which permits the number of rows and columns to be changed
    // simultaniously
    /*
    CSVMatrix BlockCopy(matrix_row_t::size_type start_column, matrix_t::size_type end_column,
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

    void ReorderColumnBefore(const std::string& reference_column_name,
                             const std::string& column_to_move_name);

    matrix_row_t::size_type GetColumnIndex(const std::string& column_name) const;

    bool ColumnExists(const std::string& column_name) const;

    void SetColumn(const std::string& column_name, const std::string& value);

    // TODO: first_row_is_header_row not used?
    void SetColumn(const matrix_t::size_type column_index, const std::string& value, const bool first_row_is_header_row);

    void DeleteColumn(const std::string& column_name);

    // is it row major or column major?
    void DeleteColumn(const matrix_t::size_type column_index);

    // is it row major or column major?
    // it is row major
    // which means fix the row index to be 0, read all elements from the vector
    std::string GetColumnNamesAsString() const;

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