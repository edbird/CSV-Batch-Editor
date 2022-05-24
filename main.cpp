

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <stdexcept>




void EraseCRLF(std::string &s)
{
    s.erase(std::remove(s.begin(), s.end(), '\r' ), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\n' ), s.end());
}

template<class CharT, class Traits, class Allocator>
std::basic_istream<CharT, Traits>& GetLine(
    std::basic_istream<CharT,Traits>& input,
    std::basic_string<CharT,Traits,Allocator>& str)
{
    auto& ret = std::getline(input, str);
    EraseCRLF(str);
    return ret;
}


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

    std::vector<std::string> SplitStringByToken(std::string s, const std::string& token)
    {
        std::vector<std::string> ret;

        for(;;)
        {
            std::string::size_type pos(s.find(token));

            if(pos != std::string::npos)
            {
                // TODO: need to check pos > 0 for all calls to substr
                if(pos > 0)
                {
                    std::string subs(s.substr(0, pos));
                    ret.push_back(subs);

                    if(pos + 1 < s.size())
                    {
                        // not the last character in the string
                        s = s.substr(pos + 1);
                    }
                    else
                    {
                        // last character in string
                        // and delimiter was found
                        ret.push_back(std::string());
                        break; // TODO: no idea if this logic is correct here!
                    }
                }
                else // no longer need this?
                {
                    ret.push_back(std::string());

                    if(pos + 1 < s.size())
                    {
                        // not the last character in the string
                        s = s.substr(pos + 1);
                    }
                    else
                    {
                        // last character in string
                        // and delimiter NOT found
                        break; // TODO: no idea if this logic is correct here!
                    }
                }
            }
            else
            {
                // delimiter not found, add remaining string to vector
                ret.push_back(s);
                break;
            }
        }

        for(const auto& s : ret)
        {
            std::cout << "split string: " << s << std::endl;
        }

        return ret;
    }

    void ReadFromFile(const std::string& filename, const bool first_row_is_header_row)
    {
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

            while(GetLine(ifs, line))
            {
                std::cout << "ReadFromFile: Line: " << line << std::endl;

                // ignore null lines if at end of file TODO

                std::vector<std::string> split_string = SplitStringByToken(line, comma_token);

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
                            std::string("Error: Number of columns is ")
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
            }

            ifs.close();
        }
    }

    void SaveToFile(const std::string& filename) const
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

            ofs.flush();
            ofs.close();
        }
    }

    void SaveToFile() const
    {
        SaveToFile(m_filename);
    }

    // TODO: version of this with a value
    // And command "add column name after existing_name with value v"
    void AddColumnAfter(const std::string& column_name,
                        const std::string& new_column_name)
    {
        matrix_row_t::size_type column_index(GetColumnIndex(column_name));
        AddColumnAfter(column_index, true);
        
        // TODO: first_row_is_header_row not used?
    }

    matrix_row_t::size_type GetColumnIndex(const std::string& column_name) const
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

    bool ColumnExists(const std::string& column_name) const
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

    void SetColumn(const std::string& column_name, const std::string& value)
    {
        matrix_row_t::size_type column_index(GetColumnIndex(column_name));
        SetColumn(column_index, value, true);
    }

    // TODO: first_row_is_header_row not used?
    void SetColumn(matrix_t::size_type column_index, const std::string& value, const bool first_row_is_header_row)
    {
        if(column_index < m_columns)
        {
            for(matrix_row_t::size_type i(0); i < m_rows; ++ i)
            {
                m_matrix.at(i).at(column_index).SetString(value);
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


// generic "main program" class
// drives operation of batch csv file editor
class MainClass
{

    public:



    void SetScriptFilePath(const std::string& filepath)
    {
        m_script_file = filepath;
    }

    bool ScriptFilePathIsSet() const
    {
        return m_script_file.size() > 0;
    }

    void RunMain()
    {
        if(!ScriptFilePathIsSet())
        {
            const std::string error("Error: Script file is not set");
            m_errors.push_back(error);
            return;
        }
        else
        {
            // script file set ok
            ReadScriptFile();
        }
    }

    void ReadScriptFile()
    {
        std::ifstream ifs(m_script_file);

        // commands need to be saved for later
        // store them in this vector to be executed
        // for each input file
        std::vector<std::string> commands_list;

        if(ifs.is_open())
        {
            std::size_t line_number = 0;
            std::string line;
            while(std::getline(ifs, line))
            {
                EraseCRLF(line);

                ++ line_number;

                // check zero length line
                if(line.size() == 0) continue;
                // check for invalid length line
                if(!(line.size() > 0)) continue;

                // check comment line
                //if(line.size() > 0) // must be true
                if(line.at(0) == '#') continue;

                //std::cout << "DEBUG" << std::endl;
                //std::cout << "line: " << line_number << std::endl;
                //for(auto c: line)
                //{
                //    std::cout << "c=" << (int)c << " " << c << std::endl;
                //}

                if(line.find(command_set_base_working_directory) == 0)
                {
                    // add 1 for space
                    std::string remaining(line.substr(command_set_base_working_directory.size() + 1));

                    if(IsValidPath(remaining))
                    {
                        m_base_working_directory = remaining;
                    }
                    else
                    {
                        std::string error
                        {
                            std::string("Error: Path not valid")
                                + std::string(" ") + remaining
                        };
                        m_errors.push_back(error);
                        return;
                    }
                }
                else if(line.find(command_set_subdirectory_file) == 0)
                {
                    // add 1 for space
                    std::string remaining(line.substr(command_set_subdirectory_file.size() + 1));

                    // TODO: post error/warning if this variable is already set

                    if(IsValidFilename(remaining))
                    {
                        m_subdirectory_filename = remaining;
                    }
                    else
                    {
                        std::string error
                        {
                            std::string("Error: Filename not valid")
                                + std::string(" ") + remaining
                        };
                        m_errors.push_back(error);
                        return;
                    }
                }
                else if(line.find(command_add_subdirectory) == 0)
                {
                    // add 1 for space
                    std::string remaining(line.substr(command_add_subdirectory.size() + 1));

                    if(IsValidPath(remaining))
                    {
                        std::cout << "Adding subdirectory " << remaining << " to processing list" << std::endl;
                        m_subdirectory.push_back(remaining);
                    }
                    else
                    {
                        std::string error
                        {
                            std::string("Error: Filename not valid")
                                + std::string(" ") + remaining
                        };
                        m_errors.push_back(error);
                        return;
                    }
                }
                else if(line.find(command_set_input_filename) == 0)
                {
                    // add 1 for space
                    std::string remaining(line.substr(command_set_input_filename.size() + 1));

                    if(IsValidFilename(remaining))
                    {
                        m_input_filename = remaining;
                    }
                    else
                    {
                        std::string error
                        {
                            std::string("Error: Filename not valid")
                                + std::string(" ") + remaining
                        };
                        m_errors.push_back(error);
                        return;
                    }
                }
                else if(line.find(command_set_output_filename) == 0)
                {
                    // add 1 for space
                    std::string remaining(line.substr(command_set_input_filename.size() + 1));

                    if(IsValidFilename(remaining))
                    {
                        m_output_filename = remaining;
                    }
                    else
                    {
                        std::string error("Error: Filename not valid");
                        error += std::string(" ") + remaining;
                        m_errors.push_back(error);
                        return;
                    }
                }
                else if(line.find(command_set_case_sensitive) == 0)
                {
                    // TODO
                }
                else if(line.find(command_set_delimiter) == 0)
                {
                    // TODO
                }
                else if(line.find(command_force_delimiter_on_write) == 0)
                {
                    // TODO
                }
                else if(line.find(command_set_column) == 0)
                {
                    // commands need to be saved for later
                    // don't check for valid command here
                    // TODO: check the commands are valid here
                    commands_list.push_back(line);
                }
                else if(line.find(command_add_column) == 0)
                {
                    // commands need to be saved for later
                    // don't check for valid command here
                    // TODO: check the commands are valid here
                    commands_list.push_back(line);
                }
                else if(line.find(command_write) == 0)
                {
                    // commands need to be saved for later
                    // don't check for valid command here
                    // TODO: check the commands are valid here
                    commands_list.push_back(line);
                }
                else
                {
                    // TODO: add line number
                    std::string error
                    {
                        std::string("Error: Syntax error:")
                            + std::string(" line ")
                            + std::to_string(line_number)
                            + std::string(": ") + line
                            + std::string(".")
                    };
                    m_errors.push_back(error);
                    //return;
                }

            } // while std::getline

            // load input data
            // check base working directory and input file name are set
            if(m_base_working_directory.size() > 0)
            {
                if(m_input_filename.size() > 0)
                {
                    // the base directory is known
                    // and the name of the input file(s) are known
                    // now load the list of subdirectories

                    // load list of subdirectories from file if filename
                    // of file containing subdirs is valid
                    if(m_subdirectory_filename.size() > 0)
                    {
                        std::ifstream ifs(m_subdirectory_filename);

                        if(!ifs.is_open())
                        {
                            // do nothing
                            std::cerr << "Could not open subdirectory list file " << m_subdirectory_filename << std::endl;
                        }
                        else
                        {
                            std::string line;
                            while(GetLine(ifs, line))
                            {
                                std::cout << "Got line: " << line << std::endl;
                                if(IsValidPath(line))
                                {
                                    m_subdirectory.push_back(line);
                                }
                            }
                            std::cout << "Finished reading subdirectory file " << m_subdirectory_filename << std::endl;
                        }
                    }
                    else
                    {
                        // do nothing - the filename has not been set
                        // so cannot load a list of subdirectories from file
                    }

                }
                else
                {
                    std::string e
                    {
                        std::string("Error: Input filename not set")
                    };
                    throw std::runtime_error(e);
                }
            }
            else
            {
                std::string e
                {
                    std::string("Error: Base working directory not set")
                };
                throw std::runtime_error(e);
            }


            if(m_subdirectory.size() > 0)
            {
                for(const auto& subdirectory : m_subdirectory)
                {
                    const std::string path_delimiter{"/"};

                    std::string full_path
                    {
                        m_base_working_directory + path_delimiter + subdirectory + path_delimiter + m_input_filename
                    };

                    std::cout << "Full path of data file to read: " << full_path << std::endl;

                    // load input data
                    CSVMatrix m;
                    m.ReadFromFile(full_path, true); // TODO: defaulting to first line as header here, change to allow configuration input
                    m_csvmatrix.emplace_back(std::move(m));
                }

                // TODO: only do this if loading of files worked ok
                // process the commands
                for(auto command: commands_list)
                {
                    ExecuteCommand(command);
                }
            }
            else
            {
                std::cerr << "Nothing to process" << std::endl;
            }

        }
        else
        {
            std::string e
            {
                std::string("Error: Could not open script file")
                    + std::string(" ") + m_script_file
            };
            throw std::runtime_error(e);
        }
    }

    // this function executes a command on all the loaded input
    // files
    void ExecuteCommand(const std::string& command)
    {
        // interpret the command and decide what to do
        if(command.find(command_set_column) == 0)
        {
            std::string remaining(command.substr(command_set_column.size() + 1));
            //std::cout << "remaining=[" << remaining << "]" << std::endl;
            std::string column_name(GetNextWord(remaining));
            //std::cout << "column_name=[" << column_name << "]" << std::endl;
            if(column_name.size() > 0)
            {
                remaining = remaining.substr(column_name.size() + 1);
                //std::cout << "remaining=[" << remaining << "]" << std::endl;

                std::string value(GetNextWord(remaining));
                //std::cout << "value=[" << value << "]" << std::endl;
                if(value.size() > 0)
                {
                    std::cout << "parsed command: " << command_set_column << " " << column_name << " " << value << std::endl;

                    for(auto& csvmatrix : m_csvmatrix)
                    {
                        csvmatrix.SetColumn(column_name, value);
                    }
                }
            }
            else
            {
                std::string e
                {
                    std::string("Error: Expected column name after ")
                        + command_set_column
                };
                throw std::runtime_error(e);
            }
        }
        else if(command.find(command_add_column) == 0)
        {
            std::string remaining{command.substr(command_add_column.size() + 1)};

            std::string column_name{GetNextWord(remaining)};

            if(column_name.size() > 0)
            {
                remaining = remaining.substr(column_name.size() + 1);

                std::string locator{GetNextWord(remaining)};

                if(locator.size() > 0)
                {
                    if(locator == string_before || locator == string_after)
                    {
                        remaining = remaining.substr(locator.size() + 1);
                        //std::cout << "remaining=[" << remaining << "]" << std::endl;

                        std::string existing_column_name{GetNextWord(remaining)};

                        if(existing_column_name.size() > 0)
                        {
                            std::cout << "parsed command: "
                                      << command_add_column << " "
                                      << column_name << " "
                                      << locator << " "
                                      << existing_column_name << std::endl;
                            
                            for(auto& csvmatrix : m_csvmatrix)
                            {
                                if(locator == string_after)
                                {
                                    csvmatrix.AddColumnAfter(existing_column_name, column_name, value);
                                }
                                else if(locator == string_before)
                                {
                                    csvmatrix.AddColumnBefore(existing_column_name, column_name, value);
                                }
                            }
                        }
                        else
                        {
                            std::string e
                            {
                                std::string("Error: Expected column name after locator \"")
                                    + locator
                                    + std::string("\"")
                            };
                            throw std::runtime_error(e);
                        }
                    }
                    else
                    {
                        std::string e
                        {
                            std::string("Error: Expected ")
                                + string_before
                                + std::string(" or ")
                                + string_after
                                + std::string(" after ")
                                + column_name
                        };
                        throw std::runtime_error(e);
                    }
                }

            }
        }
        else
        {
            std::cerr << "Error: Command " << command << " is not implemented" << std::endl;
            // TODO change for an exception
        }

        // execute the command for each object in m_csvmatrix

        // TODO: wrap the execution statements in a try-catch block
        // and report errors, marking file as "out of operation" if
        // an error occurs and do not attempt to process subsequent
        // commands on it if an error occurs
    }

    std::string GetNextWord(const std::string &s)
    {
        auto pos{s.find(" ")};
        if(pos == std::string::npos)
        {
            return s;
        }
        else
        {
            return s.substr(0, pos);
        }
    }

    bool IsValidPath(const std::string& path)
    {
        const std::string valid_chars(  "abcdefghijklmnopqrstuvwxyz\
                                         ABCDEFGHIJKLMNOPQRSTUVWXYZ\
                                         1234567890\
                                         ./\\-_+~");

        auto lambda = [&valid_chars](const char a)
        {
            return valid_chars.find(a) != std::string::npos;
        };

        return std::all_of(path.cbegin(), path.cend(), lambda);
    }

    bool IsValidFilename(const std::string &filename)
    {
        const std::string valid_chars(  "abcdefghijklmnopqrstuvwxyz\
                                         ABCDEFGHIJKLMNOPQRSTUVWXYZ\
                                         1234567890\
                                         .-_+~");

        auto lambda = [&valid_chars](const char a)
        {
            return valid_chars.find(a) != std::string::npos;
        };

        return std::all_of(filename.cbegin(), filename.cend(), lambda);
    }

    void PrintErrors(std::ostream& os) const
    {
        for(auto err: m_errors)
        {
            os << err << std::endl;
        }
    }

    private:

    std::vector<std::string> m_errors;

    std::string m_script_file;

    // base working directory relative to which operations are performed
    std::string m_base_working_directory;

    // list of subdirectories relative to base working directory
    std::list<std::string> m_subdirectory;
    // filename where subdirectory names are read from
    std::string m_subdirectory_filename;

    // input filename. the same for every subdirectory
    std::string m_input_filename;
    std::string m_output_filename; // this is usually blank, in which case the input filename is used for the output

    // all the objects which store data are contained in a list
    std::list<CSVMatrix> m_csvmatrix;



    // valid commands
    // note: for now I specify a command as an entire string
    // with spaces in it, rather than using a proper grammar
    // parser

    // commands which control the behaviour of this class
    // where to find the input files, the names of the input files,
    // etc
    static const std::string command_set_base_working_directory;
    static const std::string command_add_subdirectory;
    static const std::string command_set_subdirectory_file;
    static const std::string command_set_input_filename;
    static const std::string command_set_output_filename;

    // commands which control the behaviour of the CSVMatrix class
    // file format specifics, operations which can be performed on
    // a CSVMatrix etc
    static const std::string command_set_case_sensitive;
    static const std::string command_set_delimiter;
    static const std::string command_force_delimiter_on_write;
    static const std::string command_set_column;
    static const std::string command_add_column;
    static const std::string command_write;

    // these are not commands but elements of grammar
    static const std::string string_after;
    static const std::string string_before;


};

const std::string MainClass::command_set_base_working_directory("set base working directory");
const std::string MainClass::command_add_subdirectory("add subdirectory");
const std::string MainClass::command_set_subdirectory_file("set subdirectory file");
const std::string MainClass::command_set_input_filename("set input filename");
const std::string MainClass::command_set_output_filename("set output filename");

const std::string MainClass::command_set_case_sensitive("set case-sensitive");
const std::string MainClass::command_set_delimiter("set delimiter");
const std::string MainClass::command_force_delimiter_on_write("set force delimiter on write");
const std::string MainClass::command_set_column("set column");
const std::string MainClass::command_add_column("add column");
const std::string MainClass::command_write("write");

const std::string MainClass::string_after("after");
const std::string MainClass::string_before("before");


int main(int argc, char* argv[])
{

    MainClass my_main;

    if(argc < 2) std::cerr << "Not enough arguments" << std::endl;

    // process command line arguments
    for(std::size_t ix{1}; ix < argc; )
    {
        const std::string arg_script_file("--script-file");

        std::string arg(argv[ix]);

        if(arg == arg_script_file)
        {
            if(++ ix < argc)
            {
                std::string value(argv[ix]);
                ++ ix;

                // check if path to script file exists
                if(std::filesystem::exists(value))
                {
                    my_main.SetScriptFilePath(value);
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            std::cerr << "Error unknown argument " << arg << std::endl;
        }
    }

    // execute the actual main function
    my_main.RunMain();
    my_main.PrintErrors(std::cerr);

    return 0;
}