#include "Parser.h"
#include <iostream>
#include <string>
#include <cstddef>
#include <sstream>

Parser::Parser(const string& inputFile) {
    inFileStream.open(inputFile);
    if (!inFileStream.is_open()) {
        cerr << "Failed to open input file: " << inputFile << endl;
    }
}

Parser::~Parser() {
    if (inFileStream.is_open()) {
        inFileStream.close();
    }
}

string Parser::removeComment(string& line){
    string result;
    for(int i = 0; i < line.length(); ++i){
        if(line[i] == '/'){
            break;
        }

        result.push_back(line[i]);
    }

    return result;
}

string Parser::getWord(size_t n) const {
    istringstream iss(currentCommand);
    string word;
    for (size_t i = 0; i <= n; ++i) {
        if (!(iss >> word)) {
            return "";
        }
    }
    return word;
}

bool Parser::hasMoreCommands() {
    currentCommand.clear();
    while(inFileStream.is_open() && !inFileStream.eof()){
        getline(inFileStream, currentCommand);
        currentCommand = removeComment(currentCommand);
        if(currentCommand.length()){
            break;
        }
    }

    if(currentCommand.empty()){
        return false;
    }

    return true;
}

void Parser::advance() {
    auto commandType = g_commands.find(getWord(0));
    if(commandType == g_commands.end()){
        currentCommandType = VM_COMMAND_TYPE::C_UNKNOWN;
    }else{
        currentCommandType = commandType->second;
    }

    if(currentCommandType == VM_COMMAND_TYPE::C_ARITHMETIC){
        currentArg1 = currentCommand;
    }else{
        currentArg1 = getWord(1);
    }

    if(currentCommandType == VM_COMMAND_TYPE::C_PUSH || 
        currentCommandType == VM_COMMAND_TYPE::C_POP || 
        currentCommandType == VM_COMMAND_TYPE::C_FUNCTION || 
        currentCommandType == VM_COMMAND_TYPE::C_CALL
    ){
        currentArg2 = std::stoi(getWord(2));
    }else{
        currentArg2 = -1;
    }

    
}

VM_COMMAND_TYPE Parser::commandType() {
    return currentCommandType;
}

string Parser::arg1(){
    return currentArg1;
}

int Parser::arg2(){
    return currentArg2;
}
