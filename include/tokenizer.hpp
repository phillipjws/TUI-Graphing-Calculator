#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>

class Tokenizer {
    public:
        Tokenizer();
        std::vector<std::string> tokenize(const std::string &expression);
        void replace_variable(std::vector<std::string> &tokens, char old_var,
                              char new_var);
        std::string
        reconstruct_expression(const std::vector<std::string> &tokens);
};

#endif // TOKENIZER_HPP