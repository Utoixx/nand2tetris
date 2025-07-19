#include "CodeWriter.h"
#include <iostream>
#include <vector>
#include <filesystem>

CodeWriter::CodeWriter(const string& outPutFile , const string& inputVMFilleName) : inputVMFilleName(inputVMFilleName), labelIndex(0){
    outFileStream.open(outPutFile);
    if (!outFileStream.is_open()) {
        cerr << "Failed to open output file: " << outPutFile << endl;
    }
}

CodeWriter::~CodeWriter() {
    // TODO: Implement destructor logic
}

void CodeWriter::writeLines(const std::vector<string>& lines) {
    if (outFileStream.is_open()) {
        for (const auto& line : lines) {
            outFileStream << line << std::endl;
        }
    }
}

void CodeWriter::setFileName(const string& fileName) {
    // TODO: Implement setFileName logic
}

void CodeWriter::writeArithmetic(const string& command) {
    vector<string> asmLines;
    int arithmetricCommand = g_arithmetricMap.find(command)->second;

    asmLines.push_back("@SP");
    asmLines.push_back("M=M-1");
    asmLines.push_back("A=M");
    asmLines.push_back("D=M");

    switch (arithmetricCommand)
    {
    case VM_ARITHMETRIC_TYPE::ADD:
    {
        asmLines.push_back("@SP");
        asmLines.push_back("M=M-1");
        asmLines.push_back("A=M");
        asmLines.push_back("D=D+M");

        break;
    }
    case VM_ARITHMETRIC_TYPE::SUB:
    {
        asmLines.push_back("@SP");
        asmLines.push_back("M=M-1");
        asmLines.push_back("A=M");
        asmLines.push_back("D=M-D");

        break;
    }
    case VM_ARITHMETRIC_TYPE::NEG:
    {
        asmLines.push_back("D=-M");
        break;
    }
    case VM_ARITHMETRIC_TYPE::EQ:
    {
        asmLines.push_back("@SP");
        asmLines.push_back("M=M-1");
        asmLines.push_back("A=M");
        asmLines.push_back("D=M-D");
        asmLines.push_back("@EQ_" + std::to_string(labelIndex));
        asmLines.push_back("D;JEQ");
        asmLines.push_back("D=0");
        asmLines.push_back("@CONTINUE_" + std::to_string(labelIndex));
        asmLines.push_back("0;JMP");
        asmLines.push_back("(EQ_" + std::to_string(labelIndex) + ")");
        asmLines.push_back("D=-1");
        asmLines.push_back("(CONTINUE_" + std::to_string(labelIndex) + ")");

        ++labelIndex;
        break;
    }
    case VM_ARITHMETRIC_TYPE::GT:
    {
        asmLines.push_back("@SP");
        asmLines.push_back("M=M-1");
        asmLines.push_back("A=M");
        asmLines.push_back("D=M-D");
        asmLines.push_back("@GT_" + std::to_string(labelIndex));
        asmLines.push_back("D;JGT");
        asmLines.push_back("D=0");
        asmLines.push_back("@CONTINUE_" + std::to_string(labelIndex));
        asmLines.push_back("0;JMP");
        asmLines.push_back("(GT_" + std::to_string(labelIndex) + ")");
        asmLines.push_back("D=-1");
        asmLines.push_back("(CONTINUE_" + std::to_string(labelIndex) + ")");

        ++labelIndex;
        break;
    }
    case VM_ARITHMETRIC_TYPE::LT:
    {
        asmLines.push_back("@SP");
        asmLines.push_back("M=M-1");
        asmLines.push_back("A=M");
        asmLines.push_back("D=M-D");
        asmLines.push_back("@LT_" + std::to_string(labelIndex));
        asmLines.push_back("D;JLT");
        asmLines.push_back("D=0");
        asmLines.push_back("@CONTINUE_" + std::to_string(labelIndex));
        asmLines.push_back("0;JMP");
        asmLines.push_back("(LT_" + std::to_string(labelIndex) + ")");
        asmLines.push_back("D=-1");
        asmLines.push_back("(CONTINUE_" + std::to_string(labelIndex) + ")");

        ++labelIndex;
        break;
    }
    case VM_ARITHMETRIC_TYPE::AND:
    {
        asmLines.push_back("@SP");
        asmLines.push_back("M=M-1");
        asmLines.push_back("A=M");
        asmLines.push_back("D=D&M");
        break;
    }
    case VM_ARITHMETRIC_TYPE::OR:
    {
        asmLines.push_back("@SP");
        asmLines.push_back("M=M-1");
        asmLines.push_back("A=M");
        asmLines.push_back("D=D|M");
        break;
    }
    case VM_ARITHMETRIC_TYPE::NOT:
    {
        asmLines.push_back("D=!M");
        break;
    }
    default:
        break;
    }

    asmLines.push_back("@SP");
    asmLines.push_back("A=M");
    asmLines.push_back("M=D");
    asmLines.push_back("@SP");
    asmLines.push_back("M=M+1");
    writeLines(asmLines);
}

void CodeWriter::writePush(VM_SEGMENT_TYPE segmentType, int index){
    vector<string> asmLines;
    switch (segmentType)
    {
    case VM_SEGMENT_TYPE::CONSTANT:
    {
        asmLines.push_back("@" + std::to_string(index));
        asmLines.push_back("D=A");
        break;
    }
    case VM_SEGMENT_TYPE::POINTER:
    {
        if(index == 0){
            asmLines.push_back("@THIS");
        }else{
            asmLines.push_back("@THAT");
        }
        asmLines.push_back("D=M");
        break;
    }

    case VM_SEGMENT_TYPE::LOCAL:
    {
        asmLines.push_back("@LCL");
        break;
    }
    case VM_SEGMENT_TYPE::ARGUMENT:
    {
        asmLines.push_back("@ARG");
        break;
    }
    case VM_SEGMENT_TYPE::THIS:
    {
        asmLines.push_back("@THIS");
        break;
    }
    case VM_SEGMENT_TYPE::THAT:  
    {
        asmLines.push_back("@THAT");
        break;
    }
    case VM_SEGMENT_TYPE::STATIC:
    {
        std::filesystem::path fullPath = inputVMFilleName;
        string fileName = fullPath.stem().string();
        asmLines.push_back("@" + fileName + "." + std::to_string(index));
        asmLines.push_back("D=M");
        break;
    }
    case VM_SEGMENT_TYPE::TEMP:
    {
        asmLines.push_back("@R" + std::to_string(index + 5));
        asmLines.push_back("D=M");
        break;
    }
        
    default:
        break;
    }

    if(segmentType == VM_SEGMENT_TYPE::LOCAL || segmentType == VM_SEGMENT_TYPE::ARGUMENT || segmentType == VM_SEGMENT_TYPE::THIS || segmentType == VM_SEGMENT_TYPE::THAT){
        asmLines.push_back("D=M");
        asmLines.push_back("@" + std::to_string(index));
        asmLines.push_back("D=D+A");
        asmLines.push_back("A=D");
        asmLines.push_back("D=M");
    }
    
    asmLines.push_back("@SP");
    asmLines.push_back("A=M");
    asmLines.push_back("M=D");
    asmLines.push_back("@SP");
    asmLines.push_back("M=M+1");
    writeLines(asmLines);
}

void CodeWriter::writePop(VM_SEGMENT_TYPE segmentType, int index){
    vector<string> asmLines;

    switch (segmentType)
    {
    case VM_SEGMENT_TYPE::LOCAL:
    {
        asmLines.push_back("@LCL");
        asmLines.push_back("D=M");
        break;
    }
    case VM_SEGMENT_TYPE::ARGUMENT:
    {
        asmLines.push_back("@ARG");
        asmLines.push_back("D=M");
        break;
    }
    case VM_SEGMENT_TYPE::THIS:
    {
        asmLines.push_back("@THIS");
        asmLines.push_back("D=M");
        break;
    }
    case VM_SEGMENT_TYPE::THAT:
    {
        asmLines.push_back("@THAT");
        asmLines.push_back("D=M");
        break;
    }
    case VM_SEGMENT_TYPE::STATIC:
    {
        std::filesystem::path fullPath = inputVMFilleName;
        string fileName = fullPath.stem().string();
        asmLines.push_back("@" + fileName + "." + std::to_string(index));
        asmLines.push_back("D=A");
        break;
    }
    case VM_SEGMENT_TYPE::TEMP:
    {
        std::filesystem::path fullPath = inputVMFilleName;
        string fileName = fullPath.stem().string();
        asmLines.push_back("@R" + std::to_string(5 + index));
        asmLines.push_back("D=A");
        break;
    }
    case VM_SEGMENT_TYPE::POINTER:
    {
        if(index == 0){
            asmLines.push_back("@THIS");
        }else{
            asmLines.push_back("@THAT");
        }
        asmLines.push_back("D=A");
        break;
    }
    default:
        break;
    }

    
    if(segmentType == VM_SEGMENT_TYPE::LOCAL || segmentType == VM_SEGMENT_TYPE::ARGUMENT || segmentType == VM_SEGMENT_TYPE::THIS || segmentType == VM_SEGMENT_TYPE::THAT){
        asmLines.push_back("@" + std::to_string(index));
        asmLines.push_back("D=D+A");
    }

    //Store the address 
    asmLines.push_back("@R13");
    asmLines.push_back("M=D");
    //Write data from top of stack in to the address R13 point to
    asmLines.push_back("@SP");
    asmLines.push_back("M=M-1");
    asmLines.push_back("A=M");
    asmLines.push_back("D=M");
    asmLines.push_back("@R13");
    asmLines.push_back("A=M");
    asmLines.push_back("M=D");
    writeLines(asmLines);
}

void CodeWriter::writePushPop(VM_COMMAND_TYPE commandType, const string& segment, int index) {
    vector<string> asmLines;

    if(commandType != VM_COMMAND_TYPE::C_PUSH && commandType != VM_COMMAND_TYPE::C_POP){
        cerr << "Invalid command type" << endl;
        return;
    }

    int segmentType = g_segmentMap.find(segment)->second;
    if(commandType == VM_COMMAND_TYPE::C_PUSH){
        writePush((VM_SEGMENT_TYPE) segmentType, index);
    }else{
        writePop((VM_SEGMENT_TYPE) segmentType, index);
    }
}

void CodeWriter::close() {
    if (outFileStream.is_open()) {
        outFileStream.close();
    }
}