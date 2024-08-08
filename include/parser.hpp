#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include <vector>
#include <string>
#include <memory>

class Parser {
    std::vector<std::string> tokens;
    size_t current_token_index;

    std::unique_ptr<ASTNode> parse_expression();
    std::unique_ptr<ASTNode> parse_term();
    std::unique_ptr<ASTNode> parse_factor();
    std::unique_ptr<ASTNode> parse_primary();
public:
    Parser(const std::vector<std::string> &toks);
    std::unique_ptr<ASTNode> parse();
};

#endif // PARSER_HPP