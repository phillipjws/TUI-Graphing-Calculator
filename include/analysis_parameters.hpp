#ifndef ANALYSIS_PARAMETERS_HPP
#define ANALYSIS_PARAMETERS_HPP

#include "ast.hpp"
#include <filesystem>
#include <set>
#include <string>

class AnalysisParameters {
    public:
        AnalysisParameters(int start, int end, int num_samples);

        int get_start() const;
        int get_end() const;
        int get_num_samples() const;
        double get_step() const;
        double get_min_step() const;
        bool get_output_status() const;
        char get_variable() const;
        std::filesystem::path get_output_directory_path() const;
        std::string get_expression() const;
        std::set<char> get_reserved_chars() const;

        std::string display_domain() const;
        std::string display_num_step() const;
        std::string display_output_status() const;
        std::string display_variable() const;
        std::string display_output_directory_path(int max_width) const;
        std::string display_expression() const;

        void set_start(int new_start);
        void set_end(int new_end);
        void set_num_samples(int new_samples);
        void set_output_status(bool choice);
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

    private:
        int start_;
        int end_;
        int num_samples_;
        double step_;
        double min_step_;
        bool output_status_;
        char variable_;
        char old_variable_;
        std::filesystem::path output_directory_path_;
        std::string expression_;
        std::unordered_map<char, double> variable_values;
        std::set<char> reserved_chars;
        std::unique_ptr<ASTNode> ast_;
};

#endif // ANALYSIS_PARAMETERS_HPP
