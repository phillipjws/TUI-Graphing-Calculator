#include "ast.hpp"
#include <cmath>
#include <stdexcept>

// NumberNode Implementation
NumberNode::NumberNode(double val) : value(val) {}

double NumberNode::evaluate() const { return value; }

// VariableNode Implementation
VariableNode::VariableNode(char var, std::unordered_map<char, double> &vars)
    : name(var), variable_values(vars) {}

double VariableNode::evaluate() const { return variable_values[name]; }

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
