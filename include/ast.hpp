#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>

class AnalysisParameters;

// Base AST Node class
class ASTNode {
    public:
        virtual ~ASTNode() = default;
        virtual double evaluate() const = 0;
        virtual bool contains_variable(char variable) const = 0; // New method
};

// Derived class for numeric literals
class NumberNode : public ASTNode {
    public:
        NumberNode(double val);
        double evaluate() const override;
        bool contains_variable(
            char variable) const override; // Implementation in .cpp
    private:
        double value;
};

// Derived class for variables
class VariableNode : public ASTNode {
    public:
        VariableNode(char var, AnalysisParameters &params);
        double evaluate() const override;
        bool contains_variable(
            char variable) const override; // Implementation in .cpp
    private:
        char name;
        AnalysisParameters &parameters; // Reference to AnalysisParameters
};

// Derived class for binary operators
class BinaryOpNode : public ASTNode {
    public:
        BinaryOpNode(char oper, std::unique_ptr<ASTNode> l,
                     std::unique_ptr<ASTNode> r);
        double evaluate() const override;
        bool contains_variable(
            char variable) const override; // Implementation in .cpp
    private:
        char op;
        std::unique_ptr<ASTNode> left, right;
};

// Derived class for functions
class FunctionNode : public ASTNode {
    public:
        FunctionNode(const std::string &f, std::unique_ptr<ASTNode> arg);
        double evaluate() const override;
        bool contains_variable(
            char variable) const override; // Implementation in .cpp
    private:
        std::string func;
        std::unique_ptr<ASTNode> argument;
};

// Function declarations
std::unique_ptr<ASTNode>
generate_ast_from_expression(const std::string &expression,
                             AnalysisParameters &params);
double evaluate_expression(const std::unique_ptr<ASTNode> &ast);

#endif // AST_HPP
