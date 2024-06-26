#include <vector>
#include <string>

//https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c

class InputParser
{
public:
    InputParser(int &argc, char **argv);
    /// @author iain
    const std::string &getCmdOption(const std::string &option) const;
    /// @author iain
    bool cmdOptionExists(const std::string &option) const;

private:
    std::vector<std::string> tokens;
};