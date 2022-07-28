

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <stdexcept>








#include "csvmatrixelement.h"
#include "csvmatrix.h"


#include "mygetline.h"

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
                myfunctions::EraseCRLF(line);

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
                    std::string remaining(line.substr(command_set_output_filename.size() + 1));

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
                else if(line.find(command_delete_column) == 0)
                {
                    // commands need to be saved for later
                    // done't check for valid commands here
                    // TODO: check the commands are valid here
                    commands_list.push_back(line);
                }
                else if(line.find(command_reorder_column) == 0)
                {
                    // commands need to be saved for later
                    // done't check for valid commands here
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
                            while(myfunctions::GetLine(ifs, line))
                            {
                                //std::cout << "Got line: " << line << std::endl;
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


            // create an instance of CSVMatrix object for each subdirectory which will be processed 
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
                                    csvmatrix.AddColumnAfter(existing_column_name, column_name);
                                }
                                else if(locator == string_before)
                                {
                                    csvmatrix.AddColumnBefore(existing_column_name, column_name);
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
            // TODO: else
        }
        else if(command.find(command_delete_column) == 0)
        {
            std::string remaining{command.substr(command_delete_column.size() + 1)};

            std::string column_name{remaining};

            if(column_name.size() > 0)
            {

                std::cout << "parsed command: "
                            << command_delete_column << " "
                            << column_name << std::endl;

                /*
                std::cout << "debug: printing column header names" << std::endl;
                for(auto& csvmatrix : m_csvmatrix)
                {
                    std::cout << csvmatrix.GetColumnNamesAsString() << std::endl;
                }
                */

                for(auto& csvmatrix : m_csvmatrix)
                {
                    csvmatrix.DeleteColumn(column_name);
                }

            }
            // TODO: else
        }


        else if(command.find(command_reorder_column) == 0)
        {
            std::string remaining{command.substr(command_reorder_column.size() + 1)};

            //std::string column_name{GetNextWord(remaining)};

            std::string column_name_left;
            std::string column_name_right;
            std::string locator;

            const bool b1{GetTripleStringByDelimiterWord(remaining, column_name_left, string_before, column_name_right)};
            const bool b2{GetTripleStringByDelimiterWord(remaining, column_name_left, string_after, column_name_right)};

            if(b1 && !b2) locator = string_before;
            if(b2 && !b1) locator = string_after;

            if(b1 || b2)
            {
                // found
                std::cout << "input is: " << remaining << std::endl;
                std::cout << "delimiter is: " << locator << std::endl;
                std::cout << "left: [" << column_name_left << "]" << std::endl;
                std::cout << "right: [" << column_name_right << "]" << std::endl;

                if(column_name_left.size() > 0 && column_name_right.size() > 0)
                {
                    std::cout << "parsed command: "
                                << command_reorder_column << " "
                                << column_name_left << " "
                                << locator << " "
                                << column_name_right << std::endl;
                    
                    for(auto& csvmatrix : m_csvmatrix)
                    {
                        if(locator == string_after)
                        {
                            csvmatrix.ReorderColumnAfter(column_name_right, column_name_left);
                        }
                        else if(locator == string_before)
                        {
                            //TODO csvmatrix.ReorderColumnBefore(column_name_right, column_name_left);
                        }
                    }
                }
                else
                {
                    std::string e
                    {
                        std::string("Error: column name cannot be null string")
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
                        + std::string(". Command format is: reorder column COLUMN_NAME [")
                        + string_before + std::string("|") + string_after
                        + std::string("] COLUMN_NAME")
                };
                throw std::runtime_error(e);
            }
        }


        // TODO: this should be in the same place as the read command?
        else if(command.find(command_write) == 0)
        {
            if(m_output_filename.size() > 0)
            {
                const std::string path_delimiter{"/"};

                auto sd_it{m_subdirectory.cbegin()};
                auto m_it{m_csvmatrix.cbegin()};
                for(;
                    m_it != m_csvmatrix.cend();
                    ++ sd_it, ++ m_it)
                //for(auto& csvmatrix : m_csvmatrix)
                {
                    std::string subdirectory{*sd_it};

                    std::string full_path
                    {
                        m_base_working_directory + path_delimiter + subdirectory + path_delimiter + m_output_filename
                    };

                    auto& csvmatrix{*m_it};
                    csvmatrix.WriteToFile(full_path);
                }
            }
            else
            {
                const std::string path_delimiter{"/"};

                // TODO: assumes this is set
                auto sd_it{m_subdirectory.cbegin()};
                auto m_it{m_csvmatrix.cbegin()};
                for(;
                    m_it != m_csvmatrix.cend();
                    ++ sd_it, ++ m_it)
                //for(auto& csvmatrix : m_csvmatrix)
                {
                    std::string subdirectory{*sd_it};

                    std::string full_path
                    {
                        m_base_working_directory + path_delimiter + subdirectory + path_delimiter + m_input_filename
                    };

                    auto& csvmatrix{*m_it};
                    csvmatrix.WriteToFile(full_path);
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

    bool GetTripleStringByDelimiterWord(const std::string& input,
                                        std::string& left,
                                        const std::string& delimiter,
                                        std::string& right)
    {
        const auto pos{input.find(delimiter)};
        if(pos == std::string::npos)
        {
            return false;
        }
        else
        {
            left = input.substr(0, pos - 1);
            right = input.substr(pos + delimiter.length() + 1);
            return true;
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

    // Note: Assume a 1-to-1 mapping between subdirectory names
    // and CSVMatrix objects
    // it means that the index of the strings in m_subdirectory
    // must match the index of the strings in m_csvmatrix
    // otherwise the wrong objects will be written to the wrong
    // directories


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
    static const std::string command_delete_column;
    static const std::string command_reorder_column;
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
const std::string MainClass::command_delete_column("delete column");
const std::string MainClass::command_reorder_column("reorder column");
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