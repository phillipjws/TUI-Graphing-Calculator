#include "ast.hpp"
#include "analysis_parameters.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <cmath>
#include <stdexcept>

// NumberNode Implementation
NumberNode::NumberNode(double val) : value(val) {}

double NumberNode::evaluate() const { return value; }

bool NumberNode::contains_variable(char variable) const {
    return false; // Numbers do not contain variables
}

// VariableNode Implementation
VariableNode::VariableNode(char var, AnalysisParameters &params)
    : name(var), parameters(params) {}

double VariableNode::evaluate() const {
    return parameters.get_variable_value(
        name); // Get the value from AnalysisParameters
}

bool VariableNode::contains_variable(char variable) const {
    return name == variable; // Return true if this node represents the
                             // specified variable
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
        return std::pow(left_val, right_val);
    default:
        throw std::runtime_error("Unknown operator");
    }
}

bool BinaryOpNode::contains_variable(char variable) const {
    return left->contains_variable(variable) ||
           right->contains_variable(variable);
}

// FunctionNode Implementation
FunctionNode::FunctionNode(const std::string &f, std::unique_ptr<ASTNode> arg)
    : func(f), argument(std::move(arg)) {}

double FunctionNode::evaluate() const {
    double arg_val = argument->evaluate();
    if (func == "sin")
        return std::sin(arg_val);
    if (func == "cos")
        return std::cos(arg_val);
    if (func == "tan")
        return std::tan(arg_val);
    if (func == "log")
        return std::log10(arg_val);
    if (func == "ln")
        return std::log(arg_val);
    if (func == "sqrt")
        return std::sqrt(arg_val);
    throw std::runtime_error("Unknown function");
}

bool FunctionNode::contains_variable(char variable) const {
    return argument->contains_variable(variable);
}

// Function to generate AST from expression
std::unique_ptr<ASTNode>
generate_ast_from_expression(const std::string &expression,
                             AnalysisParameters &params) {
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(expression);

    Parser parser(tokens, params); // Pass AnalysisParameters to the parser
    return parser.parse();         // This returns the root node of the AST
}

// Function to evaluate the expression based on the AST
double evaluate_expression(const std::unique_ptr<ASTNode> &ast) {
    return ast->evaluate();
}
