#include "parser/parser.hpp"

bool Quartz::Parser::match(TokenType type)
{
    if (!isAtEnd() && peek().Type == type) {
        advance();
        return true;
    }
    return false;
}

std::unique_ptr<Quartz::ProgramNode> Quartz::Parser::parseProgram()
{
    auto program = std::make_unique<ProgramNode>();
    while (!isAtEnd()) {
        Token currToken = peek();
        if (currToken.Type == KEYWORD_STRATEGY)
            program->declarations.push_back(parseStrategy());
        else if (currToken.Type == KEYWORD_CONST)
            program->declarations.push_back(parseConstDeclaration());
        else
            advance();
    }
    return program;
}

std::unique_ptr<Quartz::StrategyNode> Quartz::Parser::parseStrategy()
{
    advance(); // consume 'strategy'
    Token nameToken = advance(); // strategy name (IDENTIFIER)
    std::string strategyName = std::any_cast<std::string>(nameToken.Data);
    auto strategy = std::make_unique<StrategyNode>(strategyName);
    match(OPEN_CURLY_BRACE);
    // Parse declarations inside strategy: consts and functions (init, on_data, etc.)
    while (!match(CLOSE_CURLY_BRACE) && !isAtEnd()) {
        Quartz::Token token = peek();
        if (token.Type == KEYWORD_CONST)
            strategy->body.push_back(parseConstDeclaration());
        else if (token.Type == IDENTIFIER)
        {
            auto& functionNode = parseFunctionDeclaration();
            switch (functionNode->nodeType()) {
            case NodeType::StrategyInitFunction: {
                std::unique_ptr<StrategyInitNode> initNode{static_cast<StrategyInitNode*>(functionNode.release())};
                strategy->initNode = std::move(initNode);
                break;
            }
            case NodeType::StrategyOnDataFunction: {
                std::unique_ptr<StrategyOnDataNode> onDataNode{static_cast<StrategyOnDataNode*>(functionNode.release())};
                strategy->onDataNode = std::move(onDataNode);
                break;
            }
            default:
                strategy->body.push_back(std::move(functionNode));
                break;
            }
        }
        else
            advance();
    }
    return strategy;
}

std::unique_ptr<Quartz::ConstDeclNode> Quartz::Parser::parseConstDeclaration()
{
    advance(); // consume 'const'
    Token id = advance(); // identifier
    std::string name = std::any_cast<std::string>(id.Data);
    TokenType type = NONE;
    if (match(COLON)) {
        Token typeToken = advance(); // type (e.g., STRING_KEYWORD or INT_KEYWORD)
        type = typeToken.Type;
    }
    match(EQUALS);
    Token valueToken = advance(); // literal value
    std::string value = std::any_cast<std::string>(valueToken.Data);
    match(SEMI_COLON);
    return std::make_unique<ConstDeclNode>(name, type, value);
}

std::unique_ptr<Quartz::FunctionDeclNode> Quartz::Parser::parseFunctionDeclaration()
{
    Token nameToken = advance(); // function name (e.g., init, on_data)
    std::string funcName = std::any_cast<std::string>(nameToken.Data);
    match(OPEN_BRACKET); // consume '('
    match(CLOSE_BRACKET); // consume ')'
    match(RIGHT_ARROW); // consume '->'
    Token returnTypeToken = advance(); // return type (e.g., VOID_KEYWORD)
    auto body = parseBlock();
    if (funcName == "init") {
        return std::make_unique<StrategyInitNode>(returnTypeToken.Type, std::move(body));
    }
    else if (funcName == "on_data") {
        return std::make_unique<StrategyOnDataNode>(returnTypeToken.Type, std::move(body));
    }
    return std::make_unique<FunctionDeclNode>(funcName, returnTypeToken.Type, std::move(body));
}

std::unique_ptr<Quartz::BlockNode> Quartz::Parser::parseBlock()
{
    match(OPEN_CURLY_BRACE);
    auto block = std::make_unique<BlockNode>();
    while (!match(CLOSE_CURLY_BRACE) && !isAtEnd()) {
        if (match(KEYWORD_IF)) {
            block->statements.push_back(parseIfStatement());
        }
        else {
            block->statements.push_back(parseStatement());
        }
    }
    return block;
}

std::unique_ptr<Quartz::ASTNode> Quartz::Parser::parseStatement()
{
    auto& nextToken = peek();
    if (nextToken.Type == KEYWORD_IF)
        return parseIfStatement();
    else if (nextToken.Type == KEYWORD_RETURN) {
        advance(); // consume 'return'
        match(SEMI_COLON);
        return std::make_unique<ReturnStmtNode>();
    }
    else {
        return parseExpressionStatement();
    }
}

std::unique_ptr<Quartz::ASTNode> Quartz::Parser::parseIfStatement()
{
    advance(); // consume 'if'
    match(OPEN_BRACKET);
    auto condition = parseExpression();
    match(CLOSE_BRACKET);
    auto thenBlock = parseBlock();

    std::unique_ptr<ASTNode> elseBranch = nullptr;

    if (peek().Type == KEYWORD_ELSE) {
        advance(); // consume 'else'
        if (match(KEYWORD_IF)) { // else if case
            elseBranch = parseIfStatement(); // Recursively parse "else if"
        }
        else {
            elseBranch = parseBlock(); // Final else block
        }
    }

    return std::make_unique<IfStmtNode>(std::move(condition), std::move(thenBlock), std::move(elseBranch));
}

std::unique_ptr<Quartz::ASTNode> Quartz::Parser::parseExpressionStatement()
{
    auto expr = parseExpression();
    match(SEMI_COLON);
    return std::make_unique<ExprStmtNode>(std::move(expr));
}

std::unique_ptr<Quartz::ASTNode> Quartz::Parser::parseExpression()
{
    auto left = parsePrimary();
    while (peek().Type == GREATER_THAN || peek().Type == LESS_THAN) {
        Token op = advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryExprNode>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Quartz::ASTNode> Quartz::Parser::parsePrimary()
{
    if (peek().Type == IDENTIFIER) {
        Token id = advance();
        std::string name = std::any_cast<std::string>(id.Data);
        // Check for function call (arguments in parentheses)
        if (match(OPEN_BRACKET)) {
            std::vector<std::unique_ptr<ASTNode>> args;
            while (!match(CLOSE_BRACKET) && !isAtEnd()) {
                args.push_back(parseExpression());
                match(COMMA); // Comma between arguments
            }
            return std::make_unique<CallExprNode>(name, std::move(args));
        }
        return std::make_unique<IdentifierExprNode>(name);
    }
    else if (peek().Type == STRING_VALUE || peek().Type == INT_VALUE) {
        Token lit = advance();
        return std::make_unique<LiteralExprNode>(std::any_cast<std::string>(lit.Data));
    }
    else if (peek().Type == KEYWORD_BUY) {
        advance();
        return std::make_unique<SignalNode>(BUY);
    }
    else if (peek().Type == KEYWORD_HOLD) {
        advance();
        return std::make_unique<SignalNode>(HOLD);
    }
    else if (peek().Type == KEYWORD_SELL) {
        advance();
        return std::make_unique<SignalNode>(SELL);
    }
    return nullptr;
}
