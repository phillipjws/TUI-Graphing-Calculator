#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>
#include <unordered_map>

// Base AST Node class
class ASTNode {
    public:
        virtual ~ASTNode() = default;
        virtual double evaluate() const = 0;
};

// Derived class for numeric literals
class NumberNode : public ASTNode {
    public:
        NumberNode(double val);
        double evaluate() const override;

    private:
        double value;
};

// Derived class for variables
class VariableNode : public ASTNode {
    public:
        VariableNode(char var, std::unordered_map<char, double> &vars);
        double evaluate() const override;

    private:
        char name;
        std::unordered_map<char, double> &variable_values;
};

// Derived class for binary operators
class BinaryOpNode : public ASTNode {
    public:
        BinaryOpNode(char oper, std::unique_ptr<ASTNode> l,
                     std::unique_ptr<ASTNode> r);
        double evaluate() const override;

    private:
        char op;
        std::unique_ptr<ASTNode> left, right;
};

// Derived class for functions
class FunctionNode : public ASTNode {
    public:
        FunctionNode(const std::string &f, std::unique_ptr<ASTNode> arg);
        double evaluate() const override;

    private:
        std::string func;
        std::unique_ptr<ASTNode> argument;
};

// Function declarations
std::unique_ptr<ASTNode>
generate_ast_from_expression(const std::string &expression);
double evaluate_expression(const std::unique_ptr<ASTNode> &ast,
                           double variable_value);

#endif // AST_HPP
