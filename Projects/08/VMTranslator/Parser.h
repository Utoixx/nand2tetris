#include<string>
#include<fstream>
#include<map>

using namespace std;

enum VM_COMMAND_TYPE
{
    C_UNKNOWN,
    C_ARITHMETIC = 0x0, 
    C_PUSH, 
    C_POP, 
    C_LABEL, 
    C_GOTO, 
    C_IF, 
    C_FUNCTION, 
    C_RETURN, 
    C_CALL
};

const map<string, VM_COMMAND_TYPE> g_commands= {
    //Push / pop commands
    {"push", VM_COMMAND_TYPE::C_PUSH},
    {"pop", VM_COMMAND_TYPE::C_POP},

    //Arithmetic / Logical commands
    {"add", VM_COMMAND_TYPE::C_ARITHMETIC},
    {"sub", VM_COMMAND_TYPE::C_ARITHMETIC},
    {"neg", VM_COMMAND_TYPE::C_ARITHMETIC},
    {"eq",  VM_COMMAND_TYPE::C_ARITHMETIC},
    {"gt",  VM_COMMAND_TYPE::C_ARITHMETIC},
    {"lt",  VM_COMMAND_TYPE::C_ARITHMETIC},
    {"and", VM_COMMAND_TYPE::C_ARITHMETIC},
    {"or",  VM_COMMAND_TYPE::C_ARITHMETIC},
    {"not", VM_COMMAND_TYPE::C_ARITHMETIC},

    {"goto", VM_COMMAND_TYPE::C_GOTO},
    {"if-goto", VM_COMMAND_TYPE::C_IF},
    {"label", VM_COMMAND_TYPE::C_LABEL},
    {"call", VM_COMMAND_TYPE::C_CALL},
    {"function", VM_COMMAND_TYPE::C_FUNCTION},
    {"return", VM_COMMAND_TYPE::C_RETURN}

};

class Parser
{
private:
    std::ifstream inFileStream;
    std::string currentCommand;
    VM_COMMAND_TYPE currentCommandType;
    std::string currentArg1;
    int currentArg2;
    std::string getWord(std::size_t n) const;
    string removeComment(string& line);
public:
    Parser(const string& inputFile);
    ~Parser();

    bool hasMoreCommands();
    void advance();
    VM_COMMAND_TYPE commandType();
    string arg1();
    int arg2();
};
