#ifndef QUARTZ_GENERATOR_H
#define QUARTZ_GENERATOR_H

#include <string>
#include <vector>

#include "Token.h"

class Generator {
public:
    std::string GenerateASM(const std::vector<Token>& tokens);
};


#endif //QUARTZ_GENERATOR_H
