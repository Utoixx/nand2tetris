#include<iostream>
#include<string>
#include<fstream>  
#include<cstdint>
#include<vector>
#include<map>
#include <stdexcept>
#include <bitset>

using namespace std;

const map<string, uint16_t> g_definedSymbolTable = {
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

const map<string, string> g_comp0Table = {
    {"0",   "101010"},
    {"1",   "111111"},
    {"-1",  "111010"},
    {"D",   "001100"},
    {"A",   "110000"},
    {"!D",  "001101"},
    {"!A",  "110001"},
    {"-D",  "001111"},
    {"-A",  "110011"},
    {"D+1", "011111"},
    {"A+1", "110111"},
    {"D-1", "001110"},
    {"A-1", "110010"},
    {"D+A", "000010"},
    {"D-A", "010011"},
    {"A-D", "000111"},
    {"D&A", "000000"},
    {"D|A", "010101"}
};

const map<string, string> g_comp1Table = {
    {"M",   "110000"},
    {"!M",  "110001"},
    {"-M",  "110011"},
    {"M+1", "110111"},
    {"M-1", "110010"},
    {"D+M", "000010"},
    {"D-M", "010011"},
    {"M-D", "000111"},
    {"D&M", "000000"},
    {"D|M", "010101"}
};

const map<string, string> g_destTable = {
    {"null",    "000"},
    {"M",       "001"},
    {"D",       "010"},
    {"DM",      "011"},
    {"MD",      "011"},
    {"A",       "100"},
    {"AM",      "101"},
    {"AD",      "110"},
    {"ADM",     "111"}
};

const map<string, string> g_jumpTable = {
    {"null",    "000"},
    {"JGT",     "001"},
    {"JEQ",     "010"},
    {"JGE",     "011"},
    {"JLT",     "100"},
    {"JNE",     "101"},
    {"JLE",     "110"},
    {"JMP",     "111"}
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
    Instruction(string& line){
        if(line.length() < 2){
            throw std::runtime_error("Invalid instruction length");
        }
        if(line[0] == '@'){
            _insType = InstructionType::A_INSTRUCTION;
            _symbol = line.substr(1, line.length() - 1);
            cout << endl;
        }else if(line.length() > 2 && line[0] == '(' && line[line.length() - 1] == ')'){
            _insType = InstructionType::L_INSTRUCTION;
            _symbol = line.substr(1, line.length() - 2);
            cout << endl;
        }else{
            _insType = InstructionType::C_INSTRUCTION;
            size_t pos = line.find(';');
            size_t posEqual = line.find('=');
            if(posEqual != std::string::npos){
                _dest = line.substr(0, posEqual);  
            }

            if (pos != std::string::npos) {
                if(posEqual != std::string::npos){
                    _comp = line.substr(2, pos - 2);
                }else{
                    _comp = line.substr(0, pos);
                }
                _jump = line.substr(pos + 1, line.length() - pos - 1);
                cout << endl;
            }else{
                if(posEqual != std::string::npos){
                    _comp = line.substr(posEqual + 1, line.length() - posEqual);
                }
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

class HackCInstruction : public Instruction{
public:
    string dest(){
        return g_destTable.find(Instruction::dest())->second;
    }

    string comp(int a){
        return a == 0 ? g_comp0Table.find(Instruction::comp())->second : g_comp1Table.find(Instruction::comp())->second;
    }

    string jump(){
        return g_jumpTable.find(Instruction::jump())->second;
    }
};


class SymbolTable {
private:
    map<string, int> table;
    static uint32_t nextAddress;
    SymbolTable() {}
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;
public:
    static SymbolTable& getInstance() {
        static SymbolTable instance;
        return instance;
    }

    void addEntry(const string& symbol) {
        if(g_definedSymbolTable.find(symbol) != g_definedSymbolTable.end()){
            std::cerr << "The symbol \"" << symbol <<"\" is the defined symbol" << endl;
            return;
        }

        if(table.find(symbol) == table.end()){
            table[symbol] = nextAddress;
            ++nextAddress;
        }else{
            std::cerr << "The symbol \"" << symbol <<"\" existed in the symbol table" << endl;
        }
    }

    void addEntry(const string& symbol, int address) {
        if(g_definedSymbolTable.find(symbol) != g_definedSymbolTable.end()){
            std::cerr << "The symbol \"" << symbol <<"\" is the defined symbol" << endl;
            return;
        }

        if(table.find(symbol) == table.end()){
            table[symbol] = address;
        }else{
            std::cerr << "The symbol \"" << symbol <<"\" existed in the symbol table" << endl;
        }
    }

    bool contains(const string& symbol) {
        if(g_definedSymbolTable.find(symbol) != g_definedSymbolTable.end()){
            return true;
        }

        if(table.find(symbol) != table.end()){
            return true;
        }

        return false;
    }

    int getAddress(const string& symbol) {
        auto itr = g_definedSymbolTable.find(symbol);
        if(itr != g_definedSymbolTable.end()){
            return itr->second;
        }
        
        auto it = table.find(symbol);
        if (it != table.end()) {
            return it->second;
        }

        return -1;
    }
};

uint32_t SymbolTable::nextAddress = 16;
vector<pair<int, Instruction>> g_Instructions;

string removeWhiteSpaceAndComment(string& line){
    string result;
    for(int i = 0; i < line.length(); ++i){
        if(line[i] == '/'){
            break;
        }

        if(!isspace(line[i])){
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
    ofstream outputFile(argv[2]);
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
    
    inputFile.close();

    int currentAddress = -1;
    for(auto& [line , currentInst] : g_Instructions){
        if(currentInst.instructionType() == InstructionType::A_INSTRUCTION){
            ++currentAddress;
        }else if(currentInst.instructionType() == InstructionType::L_INSTRUCTION){
            if(SymbolTable::getInstance().contains(currentInst.symbol()) == false){
                SymbolTable::getInstance().addEntry(currentInst.symbol(), currentAddress + 1);
            }
        } else if(currentInst.instructionType() == InstructionType::C_INSTRUCTION){
            ++currentAddress;
        }
    }

    vector<string> binaryInstructions;
    for(auto& [line , currentInst] : g_Instructions){
        string outLine;
        if(currentInst.instructionType() == InstructionType::A_INSTRUCTION){
            outLine.push_back('0');
            int addressValue = 0;
            try {
                addressValue = std::stoi(currentInst.symbol());
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid value for A instruction " << e.what() << std::endl;
                if(!SymbolTable::getInstance().contains(currentInst.symbol())){
                    SymbolTable::getInstance().addEntry(currentInst.symbol());
                }
                addressValue = SymbolTable::getInstance().getAddress(currentInst.symbol());
            }
            std::string binaryStr = std::bitset<15>(addressValue).to_string();
            outLine+= binaryStr;
            binaryInstructions.push_back(outLine);
        }else if(currentInst.instructionType() == InstructionType::C_INSTRUCTION){
            outLine+= "111";
            auto itr = g_comp0Table.find(currentInst.comp());
            if(itr != g_comp0Table.end()){
                outLine.push_back('0');
            }else{
                itr = g_comp1Table.find(currentInst.comp());
                outLine.push_back('1');
            }
            outLine+= itr->second;

            if(currentInst.dest().length()){
                outLine+= g_destTable.find(currentInst.dest())->second;
            }else{
                outLine+= g_destTable.find("null")->second;
            }
            
            if(currentInst.jump().length()){
                outLine+= g_jumpTable.find(currentInst.jump())->second;
            }else{
                outLine+= g_jumpTable.find("null")->second;
            }
            binaryInstructions.push_back(outLine);
        }
    }

    for (size_t i = 0; i < binaryInstructions.size(); ++i) {
        string line = binaryInstructions[i];
        outputFile << binaryInstructions[i];
        if (i != binaryInstructions.size() - 1) {
            outputFile << endl;
        }
    }
    outputFile.close();

    return 0;
}