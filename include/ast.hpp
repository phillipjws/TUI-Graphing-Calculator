#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <unordered_map>
#include <string>

// Base AST Node class
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual double evaluate() const = 0;
};

// Derived class for numeric literals
class NumberNode : public ASTNode {
    double value;
public:
    NumberNode(double val);
    double evaluate() const override;
};

// Derived class for variables
class VariableNode : public ASTNode {
    char name;
    std::unordered_map<char, double> &variable_values;
public:
    VariableNode(char var, std::unordered_map<char, double> &vars);
    double evaluate() const override;
};

// Derived class for binary operators
class BinaryOpNode : public ASTNode {
    char op;
    std::unique_ptr<ASTNode> left, right;
public:
    BinaryOpNode(char oper, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r);
    double evaluate() const override;
};

// Derived class for functions
class FunctionNode : public ASTNode {
    std::string func;
    std::unique_ptr<ASTNode> argument;
public:
    FunctionNode(const std::string &f, std::unique_ptr<ASTNode> arg);
    double evaluate() const override;
};

#endif // AST_HPP