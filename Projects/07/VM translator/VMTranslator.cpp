#include <iostream>
#include <string>
#include "CodeWriter.h"


int main(int argc, char* argv[]){
    if(argc < 3){
        cout << "Invalid argument!" << endl;
        return -1;
    }

    Parser parser(argv[1]);
    CodeWriter codeWriter(argv[2], argv[1]);

    while(parser.hasMoreCommands()){
        parser.advance();
        switch (parser.commandType())
        {
        case VM_COMMAND_TYPE::C_ARITHMETIC:
        {
            codeWriter.writeArithmetic(parser.arg1());
            break;
        }
        case VM_COMMAND_TYPE::C_PUSH:
        case VM_COMMAND_TYPE::C_POP:
        {
            codeWriter.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
            break;
        }
        
        default:
            break;
        }
        
    }

    codeWriter.close();

    return 0;
}