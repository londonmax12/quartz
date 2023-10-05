#include <iostream>
#include <fstream>
#include <string>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Generator/Generator.h"

void PrintTokens(std::vector<Quartz::Token>& tokens) {
    for (auto& token : tokens) {
        std::cout << "[" << token.Str() << "]";
        if (token.GetType() == Quartz::TokenType::ENDL) {
            std::cout << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage: example {input file}\n";
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string inputStr{};
    {
        char* buffer = (char*)malloc(size);
        if (!file.read(buffer, size)) {
            std::cerr << "Failed to read data from: " << argv[1] << "\n";
            return 1;
        }
        inputStr = buffer;
        free(buffer);
    }
    std::cout << "Performing lexical analysis...\n";
    Quartz::Lexer lexer{inputStr};
    std::vector<Quartz::Token> tokens = lexer.Tokenize();

    std::cout << "Parsing tokens...\n";
    Quartz::Parser parser{tokens};
    Quartz::NodeProgram exitNode = parser.ParseProgram();

    std::cout << "Generating assembly sources...\n";
    Quartz::Generator generator{exitNode};
    std::string outASM = generator.GenerateProgram();

    std::fstream outputFile{"out.asm", std::ios::out};
    outputFile << outASM;
    outputFile.close();

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    return 0;
}