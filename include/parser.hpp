#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "analysis_parameters.hpp"
#include <memory>
#include <string>
#include <vector>

class Parser {
public:
    Parser(const std::vector<std::string> &toks, AnalysisParameters &params);
    std::unique_ptr<ASTNode> parse();

private:
    std::vector<std::string> tokens;
    size_t current_token_index;
    AnalysisParameters &parameters;  // Use AnalysisParameters reference

    std::unique_ptr<ASTNode> parse_expression();
    std::unique_ptr<ASTNode> parse_term();
    std::unique_ptr<ASTNode> parse_factor();
    std::unique_ptr<ASTNode> parse_primary();
};

#endif // PARSER_HPP
