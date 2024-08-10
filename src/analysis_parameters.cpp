#include "analysis_parameters.hpp"
#include "ast.hpp"
#include "tokenizer.hpp"
#include <filesystem>
#include <format>
#include <iomanip>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

// Constructor definition
AnalysisParameters::AnalysisParameters(int start, int end, int num_samples)
    : output_status_(false), variable_('x'), old_variable_('x') {

    // Initialize reserved characters
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

    // Use setters to initialize members
    set_start(start);
    set_end(end);
    set_num_samples(num_samples);
    set_output_status(false); // Assuming default is false
    set_variable('x');        // Set the default variable to 'x'
    set_output_directory_path(std::filesystem::current_path().string());
    set_expression(
        "sin(x)"); // This initializes the AST with the default expression
}

// Getter for start_
int AnalysisParameters::get_start() const { return start_; }

// Getter for end_
int AnalysisParameters::get_end() const { return end_; }

// Getter for num_samples_
int AnalysisParameters::get_num_samples() const { return num_samples_; }

// Getter for step_
double AnalysisParameters::get_step() const { return step_; }

// Getter for output_status_
bool AnalysisParameters::get_output_status() const { return output_status_; }

// Getter for variable_
char AnalysisParameters::get_variable() const { return variable_; }

// Getter for variable value
double AnalysisParameters::get_variable_value(char variable) const {
    auto it = variable_values.find(variable);
    if (it != variable_values.end()) {
        return it->second;
    }
    throw std::runtime_error("Variable not found: " + std::string(1, variable));
}

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
    std::stringstream ss;

    if (step_ < 0.001) {
        ss << std::scientific << std::setprecision(1) << step_;
    } else {
        ss << std::fixed << std::setprecision(10) << step_;
        std::string step_str = ss.str();
        step_str.erase(step_str.find_last_not_of('0') + 1, std::string::npos);
        if (step_str.back() == '.') {
            step_str.pop_back();
        }
        ss.str("");
        ss << step_str;
    }

    return std::format("Number of samples: {}, Step size: {}", num_samples_,
                       ss.str());
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
        throw std::invalid_argument("Invalid Parameters: Number of samples "
                                    "must be between 100 and 100000");
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
void AnalysisParameters::set_expression(const std::string &new_expression) {
    // Check if the expression is valid before setting it
    if (!is_valid_expression(new_expression)) {
        throw std::invalid_argument(
            "Invalid expression. Ensure correct variable and syntax.");
    }

    expression_ = new_expression;

    // Attempt to generate the AST, and catch any errors
    try {
        ast_ = generate_ast_from_expression(expression_, *this);
    } catch (const std::exception &e) {
        throw std::invalid_argument(std::string("Failed to initialize AST: ") +
                                    e.what());
    }
}

// Check if the current domain parameters are valid
bool AnalysisParameters::is_valid_domain() const {
    return start_ < end_ && end_ - start_ >= 10;
}

// Check if current sample size parameters are valid
bool AnalysisParameters::is_valid_samples() const {
    return num_samples_ <= MAX_SAMPLES && num_samples_ >= MIN_SAMPLES;
}

// Check if output directory path exists and is a directory
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

// Checks if expression is valid
bool AnalysisParameters::is_valid_expression(
    const std::string &expression) const {
    Tokenizer tokenizer;
    std::vector<std::string> tokens = tokenizer.tokenize(expression);
    std::stack<std::string> parentheses_stack;
    std::set<std::string> constants = {"e", "c", "g", "h", "k",
                                       "G", "R", "i", "j", "pi"};

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto &token = tokens[i];
        // Check if the token is a variable or constant
        if (std::regex_match(token, std::regex(R"([a-zA-Z])"))) {
            if (token != std::string(1, variable_) &&
                !constants.contains(token)) {
                return false;
            }
        }
        // Check if the token is a function and followed by '('
        if (std::regex_match(
                token, std::regex(R"(\b(?:sin|cos|tan|log|ln|sqrt)\b)"))) {
            if (i + 1 >= tokens.size() || tokens[i + 1] != "(") {
                return false; // Function not followed by '('
            }
            parentheses_stack.push("(");
            ++i; // Skip the '('
            bool valid_argument = false;
            while (!parentheses_stack.empty() && ++i < tokens.size()) {
                const auto &inner_token = tokens[i];

                if (inner_token == "(") {
                    parentheses_stack.push("(");
                } else if (inner_token == ")") {
                    parentheses_stack.pop();
                    if (parentheses_stack.empty()) {
                        valid_argument = true;
                    }
                } else if (std::regex_match(inner_token,
                                            std::regex(R"([a-zA-Z])"))) {
                    if (inner_token != std::string(1, variable_) &&
                        !constants.contains(inner_token)) {
                        return false;
                    }
                } else if (std::regex_match(
                               inner_token,
                               std::regex(
                                   R"(\b(?:sin|cos|tan|log|ln|sqrt)\b)"))) {
                    valid_argument = true;
                } else if (std::regex_match(inner_token,
                                            std::regex(R"([\+\-\*/\^\d\.])"))) {
                    valid_argument = true;
                } else {
                    return false;
                }
            }
            if (!valid_argument) {
                return false; // Mismatched parentheses or invalid function
                              // argument
            }
            continue; // Skip to the next token after validating the function
        }
        if (token == ")") {
            if (parentheses_stack.empty()) {
                return false; // Unmatched closing parenthesis
            }
            parentheses_stack.pop();
        } else if (token == "(") {
            parentheses_stack.push("(");
        }
    }
    if (!parentheses_stack.empty()) {
        return false;
    }

    return true;
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
    // Update the expression to replace the old variable with the new one
    Tokenizer tokenizer;
    std::vector<std::string> tokens = tokenizer.tokenize(expression_);
    tokenizer.replace_variable(tokens, old_variable_, variable_);

    // Revalidate the updated expression
    std::string updated_expression = tokenizer.reconstruct_expression(tokens);
    if (!is_valid_expression(updated_expression)) {
        throw std::invalid_argument("Invalid Parameters: Expression is not "
                                    "valid with the new variable.");
    }

    expression_ = updated_expression;
}

// Evaluates current expression
double AnalysisParameters::evaluate_expression(double variable_value) {
    variable_values[variable_] = variable_value;
    if (!ast_) {
        throw std::runtime_error("AST is not initialized.");
    }
    return ast_->evaluate(); // Evaluate using the stored AST
}

// Updates the map with the current value of the variable
void AnalysisParameters::set_variable_value(char var, double value) {
    variable_values[var] = value;
}
