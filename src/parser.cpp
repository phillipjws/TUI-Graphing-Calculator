#include "parser.hpp"
#include "analysis_parameters.hpp"
#include <regex>
#include <stdexcept>

Parser::Parser(const std::vector<std::string> &toks,
               std::unordered_map<char, double> &vars,
               const AnalysisParameters &params)
    : tokens(toks), current_token_index(0), variable_values(vars),
      parameters(params) {}

std::unique_ptr<ASTNode> Parser::parse() { return parse_expression(); }

std::unique_ptr<ASTNode> Parser::parse_expression() {
    auto node = parse_term();
    while (current_token_index < tokens.size() &&
           (tokens[current_token_index] == "+" ||
            tokens[current_token_index] == "-")) {
        char op = tokens[current_token_index++][0];
        node =
            std::make_unique<BinaryOpNode>(op, std::move(node), parse_term());
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parse_term() {
    auto node = parse_factor();
    while (current_token_index < tokens.size() &&
           (tokens[current_token_index] == "*" ||
            tokens[current_token_index] == "/")) {
        char op = tokens[current_token_index++][0];
        node =
            std::make_unique<BinaryOpNode>(op, std::move(node), parse_factor());
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parse_factor() {
    auto node = parse_primary();
    while (current_token_index < tokens.size() &&
           tokens[current_token_index] == "^") {
        char op = tokens[current_token_index++][0];
        node = std::make_unique<BinaryOpNode>(op, std::move(node),
                                              parse_primary());
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parse_primary() {
    if (tokens[current_token_index] == "(") {
        current_token_index++;
        auto node = parse_expression();
        if (current_token_index >= tokens.size() ||
            tokens[current_token_index] != ")") {
            throw std::runtime_error("Error: Missing closing parenthesis.");
        }
        current_token_index++;
        return node;
    }

    if (std::regex_match(tokens[current_token_index],
                         std::regex(R"(\d+(\.\d+)?)"))) {
        double value = std::stod(tokens[current_token_index++]);
        return std::make_unique<NumberNode>(value);
    }

    if (std::regex_match(tokens[current_token_index],
                         std::regex(R"([a-zA-Z])"))) {
        char variable = tokens[current_token_index++][0];
        if (variable != parameters.get_variable() &&
            !parameters.get_reserved_chars().contains(variable)) {
            throw std::runtime_error("Error: Unexpected variable '" +
                                     std::string(1, variable) + "' found.");
        }
        return std::make_unique<VariableNode>(variable, variable_values);
    }

    if (std::regex_match(tokens[current_token_index],
                         std::regex(R"(\b(?:sin|cos|tan|log|ln|sqrt)\b)"))) {
        std::string func = tokens[current_token_index++];
        if (tokens[current_token_index] != "(") {
            throw std::runtime_error("Error: Expected '(' after function '" +
                                     func + "'.");
        }
        current_token_index++;
        auto argument = parse_expression();
        if (current_token_index >= tokens.size() ||
            tokens[current_token_index] != ")") {
            throw std::runtime_error(
                "Error: Expected ')' after function argument.");
        }
        current_token_index++;
        return std::make_unique<FunctionNode>(func, std::move(argument));
    }

    throw std::runtime_error("Error: Invalid token '" +
                             tokens[current_token_index] + "' in expression.");
}
