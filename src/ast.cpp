#include "ast.hpp"
#include "analysis_parameters.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <cmath>
#include <stdexcept>

// NumberNode Implementation
NumberNode::NumberNode(double val) : value(val) {}

double NumberNode::evaluate() const { return value; }

// VariableNode Implementation
VariableNode::VariableNode(char var, std::unordered_map<char, double> &vars)
    : name(var), variable_values(vars) {}

double VariableNode::evaluate() const {
    // Handle known constants
    switch (name) {
    case 'e':
        return M_E; // Natural log base
    case 'c':
        return 299792458.0; // Speed of light in vacuum (m/s)
    case 'g':
        return 9.80665; // Acceleration due to gravity (m/s^2)
    case 'h':
        return 6.62607015e-34; // Planck's constant (J·s)
    case 'k':
        return 1.380649e-23; // Boltzmann constant (J/K)
    case 'G':
        return 6.67430e-11; // Gravitational constant (m^3·kg^-1·s^-2)
    case 'R':
        return 8.314462618; // Universal gas constant (J/(mol·K))
    case 'p':
        return M_PI; // Pi, using 'p' instead of 'pi' as a char
    }

    return variable_values.at(name);
}

// BinaryOpNode Implementation
BinaryOpNode::BinaryOpNode(char oper, std::unique_ptr<ASTNode> l,
                           std::unique_ptr<ASTNode> r)
    : op(oper), left(std::move(l)), right(std::move(r)) {}

double BinaryOpNode::evaluate() const {
    double left_val = left->evaluate();
    double right_val = right->evaluate();
    switch (op) {
    case '+':
        return left_val + right_val;
    case '-':
        return left_val - right_val;
    case '*':
        return left_val * right_val;
    case '/':
        return left_val / right_val;
    case '^':
        return pow(left_val, right_val);
    default:
        throw std::runtime_error("Unknown operator");
    }
}

// FunctionNode Implementation
FunctionNode::FunctionNode(const std::string &f, std::unique_ptr<ASTNode> arg)
    : func(f), argument(std::move(arg)) {}

double FunctionNode::evaluate() const {
    double arg_val = argument->evaluate();
    if (func == "sin")
        return sin(arg_val);
    if (func == "cos")
        return cos(arg_val);
    if (func == "tan")
        return tan(arg_val);
    if (func == "log")
        return log10(arg_val);
    if (func == "ln")
        return log(arg_val);
    if (func == "sqrt")
        return sqrt(arg_val);
    throw std::runtime_error("Unknown function");
}

double evaluate_expression(const std::unique_ptr<ASTNode> &ast,
                           AnalysisParameters &params) {
    return ast->evaluate();
}

std::unique_ptr<ASTNode>
generate_ast_from_expression(const std::string &expression,
                             AnalysisParameters &params) {
    // Create the tokenizer and parser
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(expression);

    std::unordered_map<char, double> variables;

    Parser parser(tokens, variables, params);
    return parser.parse(); // This should return the root node of the AST
}