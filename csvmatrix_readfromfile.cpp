#include "csvmatrix.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>




void CSVMatrix::ReadFromFile(const std::string& filename, const bool first_row_is_header_row)
{
    m_rows = 0;
    m_columns_set = false;

    m_filename = filename;

    std::ifstream ifs(filename);

    if(!ifs.is_open())
    {
        std::string e{std::string("Error: Could not open file ") + filename};
        throw std::runtime_error(e);
    }
    else
    {
        std::string line;
        const std::string comma_token(",");
        const std::string quote_token("\"");

        std::size_t line_number = 0;

        while(GetLine(ifs, line))
        {
            ++ line_number;

            //std::cout << "ReadFromFile: Line: " << line << std::endl;

            // ignore null lines if at end of file TODO

            //std::vector<std::string> split_string = SplitStringByToken(line, comma_token);
            std::vector<std::string> split_string = SplitStringByTokenQuoted(line, comma_token, quote_token);

            if(!m_columns_set)
            {
                m_columns_set = true;
                m_columns = split_string.size();
            }
            else
            {
                if(split_string.size() != m_columns)
                {
                    std::string e
                    {
                        std::string("Error: (Line ")
                            + std::to_string(line_number)
                            + std::string(") Number of columns is ")
                            + std::to_string(split_string.size())
                            + std::string(" expected ")
                            + std::to_string(m_columns)
                    };
                    throw std::runtime_error(e);
                }
                else
                {
                }
            }

            // create new row and add to matrix
            std::vector<CSVMatrixElement> new_row;
            const bool is_header_row
            {
                first_row_is_header_row ? (m_rows == 0) : false
            };

            auto lambda = [is_header_row](const std::string& s)
            {
                return CSVMatrixElement(s, is_header_row);
            };

            std::transform(split_string.cbegin(), split_string.cend(), std::back_inserter(new_row), lambda);
            m_matrix.push_back(new_row);
            ++ m_rows;
        }

        ifs.close();
    }
}