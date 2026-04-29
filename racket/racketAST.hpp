#ifndef RACKET_AST_HPP
#define RACKET_AST_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace racket {

class ASTVisitor;

//  Todos os nós na Árvore de Sintaxe Abstrata herdam desta classe.
class ASTNode {
public:
    virtual ~ASTNode() = default;
    
    virtual void accept(ASTVisitor& visitor) = 0;

    virtual void print(int indent = 0) const = 0;
    
protected:
    void printIndent(int indent) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};

class ExprNode : public ASTNode {
public:
    virtual ~ExprNode() = default;
};

class IdNode : public ExprNode {
public:
    std::string name;
    
    explicit IdNode(const std::string& name) : name(name) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class NumberNode : public ExprNode {
public:
    double value;
    
    explicit NumberNode(double value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class StringNode : public ExprNode {
public:
    std::string value;
    
    explicit StringNode(const std::string& value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class BooleanNode : public ExprNode {
public:
    bool value;
    
    explicit BooleanNode(bool value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class QuoteNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> datum;
    
    explicit QuoteNode(std::unique_ptr<ExprNode> datum) 
        : datum(std::move(datum)) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class IfNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<ExprNode> thenBranch;
    std::unique_ptr<ExprNode> elseBranch;
    
    IfNode(std::unique_ptr<ExprNode> condition,
           std::unique_ptr<ExprNode> thenBranch,
           std::unique_ptr<ExprNode> elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class BeginNode : public ExprNode {
public:
    std::vector<std::unique_ptr<ExprNode>> expressions;
    
    explicit BeginNode(std::vector<std::unique_ptr<ExprNode>> expressions)
        : expressions(std::move(expressions)) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class AppNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> function;
    std::vector<std::unique_ptr<ExprNode>> arguments;
    
    AppNode(std::unique_ptr<ExprNode> function,
            std::vector<std::unique_ptr<ExprNode>> arguments)
        : function(std::move(function)),
          arguments(std::move(arguments)) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(IdNode& node) = 0;
    virtual void visit(NumberNode& node) = 0;
    virtual void visit(StringNode& node) = 0;
    virtual void visit(BooleanNode& node) = 0;
    virtual void visit(QuoteNode& node) = 0;
    virtual void visit(IfNode& node) = 0;
    virtual void visit(BeginNode& node) = 0;
    virtual void visit(AppNode& node) = 0;
};

} 
#endif 