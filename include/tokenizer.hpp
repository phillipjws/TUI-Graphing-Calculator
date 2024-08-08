#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>

class Tokenizer {
    public:
        Tokenizer();

        // Function to tokenize the input expression string
        std::vector<std::string> tokenize(const std::string &expression);

        // Function to replace the old variable with a new one within the
        // tokenized expression
        void replace_variable(std::vector<std::string> &tokens, char old_var,
                              char new_var);

        // Function to reconstruct the expression from tokens
        std::string
        reconstruct_expression(const std::vector<std::string> &tokens);
};

#endif // TOKENIZER_HPP