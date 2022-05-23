

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>


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

        if(ifs.is_open())
        {
            std::string line;
            while(std::getline(ifs, line))
            {
                // check zero length line
                if(line.size() == 0) continue;
                // check for invalid length line
                if(!(line.size() > 0)) continue;

                // check comment line
                //if(line.size() > 0) // must be true
                if(line.at(0) == '#') continue;

                // valid commands
                // note: for now I specify a command as an entire string
                // with spaces in it, rather than using a proper grammar
                // parser
                const std::string command_set_base_working_directory("set base working directory");

                if(line.find(command_set_base_working_directory) == 0)
                {
                    // add 1 for space
                    std::string remaining(line.substr(command_set_base_working_directory.size() + 1));

                    if(IsValidPath(remaining))
                    {

                    }
                    else
                    {
                        std::string error("Error: Path not valid");
                        error += std::string(" ") + remaining;
                        m_errors.push_back(error);
                        return;
                    }
                }
                else if(false)
                {

                }
                else
                {
                    // TODO: add line number
                    std::string error("Error: Syntax error:");
                    error += std::string(" ") + line;
                    m_errors.push_back(error);
                    return;
                }

            }
        }
        else
        {
            std::string error("Error: Could not open script file");
            error += std::string(" ") + m_script_file;
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

    private:

    std::string m_script_file;

    std::vector<std::string> m_errors;

};



int main(int argc, char* argv[])
{

    MainClass my_main;

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

                // check if path to script file exists
                if(std::filesystem::exists(value))
                {
                    my_main.SetScriptFilePath(value);
                }
            }
        }
        else
        {
            std::cerr << "Error unknown argument " << arg << std::endl;
        }
    }

    // execute the actual main function
    my_main.RunMain();

    return 0;
}