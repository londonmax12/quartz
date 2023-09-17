#include <iostream>
#include <fstream>
#include <string>

#include "Lexer.h"
#include "Generator.h"

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

    Lexer lexer{};
    std::vector<Token> tokens = lexer.Tokenize(inputStr);

    Generator generator{};
    std::string outASM = generator.GenerateASM(tokens);

    std::fstream outputFile{"out.asm", std::ios::out};
    outputFile << outASM;
    outputFile.close();

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    return 0;
}