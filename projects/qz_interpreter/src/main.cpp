#include <iostream>
#include <string>

#include <quartz/logging/logging.hpp>
#include <quartz/parser/abstractSyntaxTree.hpp>
#include <quartz/quartz.hpp>

#include "interpreter.hpp"

int main(int argc, char* argv[]) {
    std::string filename;
    std::string code;
    bool verbose = false;

    if (argc < 2) {
        Quartz::Logger::getInstance().logf(Quartz::Logger::ERROR, "Usage: %s (-f <filename> | -c <code>) [-v]", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-f") {
            if (i + 1 < argc) {
                filename = argv[++i];
            }
            else {
                Quartz::Logger::getInstance().log(Quartz::Logger::ERROR, "-f requires a filename");
                return 1;
            }
        }
        else if (arg == "-c") {
            if (i + 1 < argc) {
                code = argv[++i];
            }
            else {
                Quartz::Logger::getInstance().log(Quartz::Logger::ERROR, "-c requires code");
                return 1;
            }
        }
        else if (arg == "-v") {
            verbose = true;
        }
        else {
            Quartz::Logger::getInstance().log(Quartz::Logger::ERROR, "Unknown flag");
            return 1;
        }
    }

    if (verbose) {
        Quartz::Logger::getInstance().setPrintLevel(Quartz::Logger::DEBUG);
        Quartz::Logger::getInstance().log(Quartz::Logger::INFO, "Verbose logging mode enabled");
    }

    std::shared_ptr<Quartz::ProgramNode> program;
    if (!filename.empty()) {
        program = Quartz::run_file(filename.c_str());
    }
    else if (!code.empty()) {
        program = Quartz::run_code(code.c_str());
    }
    else {
        Quartz::Logger::getInstance().log(Quartz::Logger::ERROR, "No filename or code provided");
        return 1;
    }

    Quartz::Interpreter interpreter = Quartz::Interpreter(program);
    interpreter.interpret();

    return 0;
}