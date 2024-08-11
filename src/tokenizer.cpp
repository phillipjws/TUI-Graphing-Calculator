#include "tokenizer.hpp"
#include <regex>

Tokenizer::Tokenizer() {}

std::vector<std::string> Tokenizer::tokenize(const std::string &expression) {
    std::vector<std::string> tokens;

    std::regex token_pattern(
        R"(\bpi\b|\d+(\.\d+)?|[a-zA-Z_]\w*|[+\-*/^()]|\b(?:sin|cos|tan|arcsin|arccos|arctan|log|ln|sqrt)\b)");
    auto words_begin = std::sregex_iterator(expression.begin(),
                                            expression.end(), token_pattern);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        tokens.push_back(i->str());
    }

    return tokens;
}

void Tokenizer::replace_variable(std::vector<std::string> &tokens, char old_var,
                                 char new_var) {
    std::string old_var_str(1, old_var);
    std::string new_var_str(1, new_var);

    for (std::string &token : tokens) {
        // Replace only if the token is exactly the old variable
        if (token == old_var_str) {
            token = new_var_str;
        }
    }
}

std::string
Tokenizer::reconstruct_expression(const std::vector<std::string> &tokens) {
    std::string expression;

    for (const std::string &token : tokens) {
        expression += token;
    }

    return expression;
}
