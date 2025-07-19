#pragma once
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "Parser.h"

using namespace std;

enum VM_SEGMENT_TYPE{
    SEG_UNKNOWN = 0x0,
    CONSTANT,
    LOCAL,
    ARGUMENT,
    THIS,
    THAT,
    STATIC,
    TEMP,
    POINTER
};

enum VM_ARITHMETRIC_TYPE{
    ARITH_UNKNOWN = 0x0,
    ADD,
    SUB,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT
};

const map<string, int> g_segmentMap = {
    {"constant",    VM_SEGMENT_TYPE::CONSTANT},
    {"local",       VM_SEGMENT_TYPE::LOCAL},
    {"argument",    VM_SEGMENT_TYPE::ARGUMENT},
    {"this",        VM_SEGMENT_TYPE::THIS},
    {"that",        VM_SEGMENT_TYPE::THAT},
    {"static",      VM_SEGMENT_TYPE::STATIC},
    {"temp",        VM_SEGMENT_TYPE::TEMP},
    {"pointer",     VM_SEGMENT_TYPE::POINTER}
};

const map<string, int> g_arithmetricMap = {
    {"add", VM_ARITHMETRIC_TYPE::ADD},
    {"sub", VM_ARITHMETRIC_TYPE::SUB},
    {"neg", VM_ARITHMETRIC_TYPE::NEG},
    {"eq",  VM_ARITHMETRIC_TYPE::EQ},
    {"gt",  VM_ARITHMETRIC_TYPE::GT},
    {"lt",  VM_ARITHMETRIC_TYPE::LT},
    {"and", VM_ARITHMETRIC_TYPE::AND},
    {"or",  VM_ARITHMETRIC_TYPE::OR},
    {"not", VM_ARITHMETRIC_TYPE::NOT}
};

class CodeWriter{
private:
    std::ofstream outFileStream;
    string inputVMFilleName;
    int labelIndex;
    void writeLines(const vector<string>& lines);
    void writePush(VM_SEGMENT_TYPE segmentType, int index);
    void writePop(VM_SEGMENT_TYPE segmentType, int index);
public:
    CodeWriter(const string& outPutFile, const string& inputVMFilleName);
    ~CodeWriter();

    void setFileName(const string& fileName);
    void writeArithmetic(const string& command);
    void writePushPop(VM_COMMAND_TYPE commandType, const string& segment, int index);
    void close();
};