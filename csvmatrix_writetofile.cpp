#include "csvmatrix.h"


#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>


void CSVMatrix::WriteToFile() const
{
    WriteToFile(m_filename);
}

void CSVMatrix::WriteToFile(const std::string& filename) const
{
    std::ofstream ofs(filename);

    if(!ofs.is_open())
    {
        std::string e
        {
            std::string("Error: Could not open file ") + filename
        };
        throw std::runtime_error(e);
    }
    else
    {
        for(std::size_t row_ix{0}; row_ix < m_rows; ++ row_ix)
        {
            if(row_ix > 0)
            {
                ofs << std::endl;
            }

            for(std::size_t col_ix{0}; col_ix < m_columns; ++ col_ix)
            {
                std::string out_string;
                const std::string token_comma(",");

                out_string = m_matrix[row_ix][col_ix].GetString();

                if(col_ix > 0)
                {
                    ofs << token_comma;
                }

                ofs << out_string;
            }
        }
        //std::cout << "write " << m_rows << " rows" << std::endl;

        ofs.flush();
        ofs.close();
    }
}