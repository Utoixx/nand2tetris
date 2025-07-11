#include<iostream>
#include<string>
#include<fstream>  
#include<cstdint>
#include<vector>
#include<map>
#include <stdexcept>

using namespace std;

map<string, uint16_t> g_symbolTable = {
    {"R0", 0},
    {"R1", 1},
    {"R2", 2},
    {"R3", 3},
    {"R4", 4},
    {"R5", 5},
    {"R6", 6},
    {"R7", 7},
    {"R8", 8},
    {"R9", 9},
    {"R10", 10},
    {"R11", 11},
    {"R12", 12},
    {"R13", 13},
    {"R14", 14},
    {"R15", 15},
    {"SCREEN", 16384},
    {"KBD", 24576},
    {"SP", 0},
    {"LCL", 1},
    {"ARG", 2},
    {"THIS", 3},
    {"THAT", 4}
};

enum InstructionType {
    A_INSTRUCTION,
    C_INSTRUCTION,
    L_INSTRUCTION
};

class Instruction{
private:
    InstructionType _insType;
    string _symbol;
    string _dest;
    string _comp;
    string _jump;
public:
    Instruction(string& hackInstruction){
        if(hackInstruction.length() < 2){
            throw std::runtime_error("Invalid instruction length");
        }
        if(hackInstruction[0] == '@'){
            _insType = InstructionType::A_INSTRUCTION;
            _symbol = hackInstruction.substr(1, hackInstruction.length() - 1);
            cout << endl;
        }else if(hackInstruction.length() > 2 && hackInstruction[0] == '(' && hackInstruction[hackInstruction.length() - 2] == ')'){
            _insType = InstructionType::L_INSTRUCTION;
            _symbol = hackInstruction.substr(1, hackInstruction.length() - 2);
            cout << endl;
        }else{
            _insType = InstructionType::C_INSTRUCTION;
            _dest = hackInstruction.substr(0, 1);
            size_t pos = hackInstruction.find(';');
            if (pos != std::string::npos) {
                _comp = hackInstruction.substr(2, pos - 2);
                _jump = hackInstruction.substr(pos + 1, hackInstruction.length() - pos - 1);
                cout << endl;
            }else{
                _comp = hackInstruction.substr(2, pos - 2);
                cout << endl;
            }
        }
    }

    InstructionType instructionType(){
        return _insType;
    }

    string symbol(){
        return _symbol;
    }

    string dest(){
        return _dest;
    }

    string comp(){
        return _comp;
    }

    string jump(){
        return _jump;
    }
};

uint32_t g_symbolTableIndex = 16;
vector<pair<int, Instruction>> g_Instructions;

string removeWhiteSpaceAndComment(string& line){
    string result;
    for(int i = 0; i < line.length(); ++i){
        if(line[i] == '/'){
            break;
        }

        if(!isspace(i)){
            result.push_back(line[i]);
        }
    }

    return result;
}

int main(int argc, char *argv[]){
    if(argc < 3){
        cout << "Invalid argument!" << endl;
        return -1;
    }

    ifstream inputFile(argv[1]);
    ifstream outputFile(argv[2]);
    if(!inputFile.is_open()){
        std::cerr << "Error opening input file: " << argv[1] << endl;
        return -1;
    }

    string line;
    int originLine = 0;
    while(getline(inputFile, line)){
        line = removeWhiteSpaceAndComment(line);
        if(line.length()){
            g_Instructions.push_back({originLine, Instruction(line)});
        }
        ++originLine;
    }

    --originLine;
    return 0;
}