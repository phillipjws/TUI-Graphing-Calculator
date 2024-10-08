#ifndef ANALYSIS_PARAMETERS_HPP
#define ANALYSIS_PARAMETERS_HPP

#include "ast.hpp"
#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>

class AnalysisParameters {
    public:
        AnalysisParameters(int start, int end, int num_samples);

        const int get_start() const;
        const int get_end() const;
        const int get_num_samples() const;
        const double get_step() const;
        const char get_variable() const;
        const double get_variable_value(char variable) const;
        const std::filesystem::path get_output_directory_path() const;
        const std::string get_expression() const;
        const std::set<char> get_reserved_chars() const;

        std::string display_domain() const;
        std::string display_num_step() const;
        std::string display_variable() const;
        std::string display_output_directory_path(int max_width) const;
        std::string display_expression() const;

        void set_start(int new_start);
        void set_end(int new_end);
        void set_num_samples(int new_samples);
        void set_variable(char new_variable);
        void set_output_directory_path(std::string new_dir);
        void set_expression(const std::string &new_expression);

        bool is_valid_domain() const;
        bool is_valid_samples() const;
        bool is_valid_output_path() const;
        bool is_valid_variable() const;
        bool is_valid_expression(const std::string &expression) const;

        void update_step();
        void update_expression();

        double evaluate_expression(double variable_value);

        void set_variable_value(char variable, double value);

        static const int MAX_SAMPLES = 100000;
        static const int MIN_SAMPLES = 100;

    private:
        int start_;
        int end_;
        int num_samples_;
        double step_;
        char variable_;
        char old_variable_;
        std::filesystem::path output_directory_path_;
        std::string expression_;
        std::unordered_map<char, double>
            variable_values; // Stores variable values
        std::set<char> reserved_chars;
        std::unique_ptr<ASTNode> ast_;
};

#endif // ANALYSIS_PARAMETERS_HPP
