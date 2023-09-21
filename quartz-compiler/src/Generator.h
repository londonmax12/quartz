#ifndef QUARTZ_GENERATOR_H
#define QUARTZ_GENERATOR_H

#include <string>
#include <vector>

#include "Token.h"
#include "ParserNode.h"

class Generator {
public:
    Generator(ExitNode source);

    std::string GenerateASM();
private:
    ExitNode m_Source;
};


#endif //QUARTZ_GENERATOR_H
