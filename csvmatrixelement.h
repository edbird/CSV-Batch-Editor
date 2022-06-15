#ifndef CSVMATRIX_ELEMENT_H
#define CSVMATRIX_ELEMENT_H

#include <string>

class CSVMatrixElement
{

    public:

    CSVMatrixElement(const std::string& data, const bool is_header_row)
        : m_data(data)
        , m_is_header_row(is_header_row)
    {

    }

    virtual
    ~CSVMatrixElement()
    {

    }
    

    void SetHeaderRow(const bool is_header_row)
    {
        m_is_header_row = is_header_row;
    }

    std::string GetString() const
    {
        return m_data;
    }

    void SetString(std::string s)
    {
        m_data = std::move(s);
    }

    private:

    std::string m_data;

    // assumed that there may be at most 1 header row in the CSV file,
    // and that this will be the first row.
    // in such case, all elements should have this variable set to true
    bool m_is_header_row;

};

#endif // CSVMATRIX_ELEMENT_H