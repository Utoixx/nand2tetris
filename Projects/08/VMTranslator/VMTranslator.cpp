#include <iostream>
#include <string>
#include "CodeWriter.h"
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

int Translate(const string& fileName){
    Parser parser(fileName);
    fs::path filePath = fileName;
    filePath.replace_extension(".asm");
    CodeWriter codeWriter(filePath.string(), fileName);

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
}

int main(int argc, char* argv[]){
    if(argc < 3){
        cout << "Invalid argument!" << endl;
        return -1;
    }

    std::string pathStr = argv[1];
    fs::path path(pathStr);

    if (fs::exists(path)) {
        if (fs::is_regular_file(path)) {
            Translate(path.string());
        } else if (fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".vm") {
                    Translate(entry.path().string());
                }
            }
        } else {
            std::cout << "It exists but is neither a file nor a directory.\n";
        }

    } else {
        std::cout << "Path is not valid (does not exist).\n";
    }

    

    return 0;
}