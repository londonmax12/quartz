#include <iostream>
#include <string>
#include <fstream>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Generator/Generator.h"
#include "Logging/Logging.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            LOG_FATAL("Incorrect Usage: quartz-compiler {Input File}");
            return 1;
        }

        auto start = std::chrono::high_resolution_clock::now();

        LOG_INFO("Performing lexical analysis...");
        Quartz::Lexer lexer{argv[1]};
        Quartz::Lexer::LexerOut lexerResult = lexer.Tokenize();

        LOG_INFO("Parsing tokens...");
        Quartz::Parser parser{lexerResult.Tokens};
        Quartz::NodeProgram exitNode = parser.ParseProgram();

        LOG_INFO("Generating assembly source...");
        Quartz::Generator generator{exitNode};
        std::string outASM = generator.GenerateProgram();

        std::fstream outputFile{"out.asm", std::ios::out};
        outputFile << outASM;
        outputFile.close();

        system("nasm -felf64 out.asm");
        system("ld -o out out.o");

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        LOG_INFO("Took {} milliseconds to compile", duration.count());

        return 0;
    } catch (const Quartz::LexerException& e) {
        if (e.GetToken().GetType() != Quartz::TokenType::NONE) {
            LOG_FATAL("Failed to perform lexical analysis: {}", e.what());
            LOG_FATAL("{}", e.GetToken());
        }
        else
            LOG_FATAL("Failed to perform lexical analysis: {}", e.what());
        return 1;
    } catch (const Quartz::ParserException& e) {
        if (e.GetToken().GetType() != Quartz::TokenType::NONE) {
            LOG_FATAL("Failed to perform lexical analysis");
            LOG_FATAL("{}: {}", e.GetToken(),  e.what());
        }
        else
            LOG_FATAL("Failed to perform lexical analysis: {}", e.what());
        return 1;
    } catch (const Quartz::GeneratorException& e) {
        LOG_FATAL("Generator failed: {}", e.what());
        return 1;
    }
}