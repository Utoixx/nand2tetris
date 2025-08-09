#include <iostream>
#include <string>
#include "CodeWriter.h"
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

void Translate(CodeWriter& codeWriter, const string& fileName){
    Parser parser(fileName);
    codeWriter.setFileName(fileName);
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
        case VM_COMMAND_TYPE::C_LABEL:
        {
            codeWriter.writeLabel(parser.arg1());
            break;
        }
        case VM_COMMAND_TYPE::C_GOTO:
        {
            codeWriter.writeGoto(parser.arg1());
            break;
        }
        case VM_COMMAND_TYPE::C_IF:{
            codeWriter.writeIf(parser.arg1());
            break;
        }
        case VM_COMMAND_TYPE::C_FUNCTION:
        {
            codeWriter.writeFuncion(parser.arg1(), parser.arg2());
            break;
        }
        case VM_COMMAND_TYPE::C_CALL:
        {
            codeWriter.writeCall(parser.arg1(), parser.arg2());
            break;
        }
        case VM_COMMAND_TYPE::C_RETURN:
        {
            codeWriter.writeReturn();
        }
        
        default:
            break;
        }
        
    }
}

int main(int argc, char* argv[]){
    std::string outPutFile = argv[1];
    std::string newExtension = ".asm";
    bool noMainFunction = false;

    if(argc < 2){
        cout << "Invalid argument!" << endl;
        return -1;
    }

    if(argc == 3){
        string arg2 = argv[2];
        if(arg2.compare("-f") != 0){
            cout << "Invalid argument!" << endl;
            return -1;
        }

        noMainFunction = true;
    }

    // Find the position of the last dot
    size_t lastDotPos = outPutFile.find_last_of('.');
    if (lastDotPos != std::string::npos) {
        // Replace the extension
        outPutFile = outPutFile.substr(0, lastDotPos) + newExtension;
    } else {
        // If no extension exists, simply append the new one
        outPutFile += newExtension;
    }
    
    fs::path path(argv[1]);
    CodeWriter codeWriter(outPutFile, noMainFunction);
    if (fs::exists(path)) {
        if (fs::is_regular_file(path)) {
            Translate(codeWriter, argv[1]);
        } else if (fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".vm") {
                    Translate(codeWriter, entry.path().string());
                }
            }
        } else {
            std::cout << "It exists but is neither a file nor a directory.\n";
        }

    } else {
        std::cout << "Path is not valid (does not exist).\n";
    }

    codeWriter.close();

    std::cout << "Successfule! Out put file is: " + outPutFile << endl; 

    return 0;
}