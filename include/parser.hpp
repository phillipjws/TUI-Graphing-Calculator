#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Parser {
    public:
        Parser(const std::vector<std::string> &toks,
               std::unordered_map<char, double> &vars);
        std::unique_ptr<ASTNode> parse();

    private:
        std::vector<std::string> tokens;
        size_t current_token_index;
        std::unordered_map<char, double>
            &variable_values; // Reference to variable values map

        std::unique_ptr<ASTNode> parse_expression();
        std::unique_ptr<ASTNode> parse_term();
        std::unique_ptr<ASTNode> parse_factor();
        std::unique_ptr<ASTNode> parse_primary();
};

#endif // PARSER_HPP