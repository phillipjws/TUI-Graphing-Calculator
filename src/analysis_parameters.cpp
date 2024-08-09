#include "analysis_parameters.hpp"
#include "ast.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

// Constructor definition
AnalysisParameters::AnalysisParameters(int start, int end, int num_samples)
    : start_(start), end_(end), num_samples_(num_samples), min_step_(0.000001),
      output_status_(false), variable_('x'), old_variable_('x'),
      expression_("sin(x)") {
    reserved_chars = {
        'e', // Base of natural logarithm
        'c', // Speed of light in a vacuum
        'g', // Acceleration due to gravity
        'h', // Planck’s constant
        'k', // Boltzmann constant
        'G', // Gravitational constant
        'R', // Universal gas constant
        'i', // Imaginary unit
        'j', // Alternative imaginary unit (engineering)
    };
    set_output_directory_path(std::filesystem::current_path());
    if (!is_valid_domain()) {
        throw std::invalid_argument(
            "Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Getter for start_
int AnalysisParameters::get_start() const { return start_; }

// Getter for end_
int AnalysisParameters::get_end() const { return end_; }

// Getter for num_samples_
int AnalysisParameters::get_num_samples() const { return num_samples_; }

// Getter for step_
double AnalysisParameters::get_step() const { return step_; }

// Getter for min_step_
double AnalysisParameters::get_min_step() const { return min_step_; }

// Getter for output_status_
bool AnalysisParameters::get_output_status() const { return output_status_; }

// Getter for variable_
char AnalysisParameters::get_variable() const { return variable_; }

// Getter for output_directory_path_
std::filesystem::path AnalysisParameters::get_output_directory_path() const {
    return output_directory_path_;
}

// Getter for expression_
std::string AnalysisParameters::get_expression() const { return expression_; }

// Getter for reserved_chars
std::set<char> AnalysisParameters::get_reserved_chars() const {
    return reserved_chars;
}

// Display the domain as a string
std::string AnalysisParameters::display_domain() const {
    return std::format("Domain: [{}, {}]", start_, end_);
}

// Display the number of samples and step size
std::string AnalysisParameters::display_num_step() const {
    return std::format("Number of samples: {}, Step size: {:.6f}", num_samples_,
                       step_);
}

// Display the output status as a string
std::string AnalysisParameters::display_output_status() const {
    if (output_status_) {
        return "Enable output file is: On";
    }
    return "Enable output file is: Off";
}

// Display the current variable as a string
std::string AnalysisParameters::display_variable() const {
    return std::format("Independent variable: {}", variable_);
}

// Display the current output directory path as a string
std::string
AnalysisParameters::display_output_directory_path(int max_width) const {
    if (output_status_) {
        std::string prompt = "Current output directory is: ";
        std::string path = output_directory_path_.string();

        int available_space = max_width - prompt.length();
        if (path.length() > available_space) {
            path = "..." + path.substr(path.length() - available_space + 3);
        }

        return std::format("{}{}", prompt, path);
    }
    return "Enable output file is off, will not save any output.";
}

// Display the current expression as a string
std::string AnalysisParameters::display_expression() const {
    return std::format("f({}) = {}", variable_, expression_);
}

// Setter for start_
void AnalysisParameters::set_start(int new_start) {
    start_ = new_start;
    if (!is_valid_domain()) {
        throw std::invalid_argument(
            "Invalid Parameters: Start must be less than end.");
    }
    update_step();
}

// Setter for end_
void AnalysisParameters::set_end(int new_end) {
    end_ = new_end;
    if (!is_valid_domain()) {
        throw std::invalid_argument(
            "Invalid Parameters: Start must be less than end.");
    }
    update_step();
}

// Setter for num_samples_
void AnalysisParameters::set_num_samples(int new_samples) {
    num_samples_ = new_samples;
    update_step();
    if (!is_valid_samples()) {
        throw std::invalid_argument("Invalid Parameters: Step size too small. "
                                    "Increase the number of samples.");
    }
}

// Setter for output_status_
void AnalysisParameters::set_output_status(bool choice) {
    output_status_ = choice;
}

// Setter for variable_
void AnalysisParameters::set_variable(char new_variable) {
    old_variable_ = variable_;
    variable_ = new_variable;
    if (!is_valid_variable()) {
        throw std::invalid_argument(
            "Invalid Parameters: Variable cannot be a reserved character.");
    }
    update_expression();
}

// Setter for output_directory_path_
void AnalysisParameters::set_output_directory_path(std::string new_dir) {
    if (!new_dir.empty() && new_dir.back() != '/') {
        output_directory_path_ = new_dir + "/";
    } else {
        output_directory_path_ = new_dir;
    }
    if (!is_valid_output_path()) {
        throw std::invalid_argument(
            "Invalid Parameters: Must be an existing directory path.");
    }
}

// Setter for expression_
void AnalysisParameters::set_expression(std::string new_expression) {
    expression_ = new_expression;
}

// Check if the current domain parameters are valid
bool AnalysisParameters::is_valid_domain() const {
    return start_ < end_ && end_ - start_ >= 10;
}

// Check if current sample size parameters are valid
bool AnalysisParameters::is_valid_samples() const {
    return step_ >= min_step_ && num_samples_ > 0;
}

// Check if output diresctory path exists and is a directory
bool AnalysisParameters::is_valid_output_path() const {
    if (std::filesystem::exists(output_directory_path_) &&
        std::filesystem::is_directory(output_directory_path_)) {
        return true;
    }
    return false;
}

// Checks if variable is not reserved
bool AnalysisParameters::is_valid_variable() const {
    return !reserved_chars.contains(variable_);
}

// Update step based on current parameters
void AnalysisParameters::update_step() {
    if (num_samples_ > 0) {
        step_ = static_cast<double>(end_ - start_) / num_samples_;
    } else {
        step_ = std::numeric_limits<double>::infinity();
    }
}

// Update expression based on new variable
void AnalysisParameters::update_expression() {
    Tokenizer tokenizer;
    std::vector<std::string> tokens = tokenizer.tokenize(expression_);
    tokenizer.replace_variable(tokens, old_variable_, variable_);

    Parser parser(tokens, variable_values, *this);
    ast_ = parser.parse();

    expression_ = tokenizer.reconstruct_expression(tokens);
}

// Evaluates current expression
double AnalysisParameters::evaluate_expression(double variable_value) {
    variable_values[variable_] = variable_value;
    return ast_->evaluate();
}
