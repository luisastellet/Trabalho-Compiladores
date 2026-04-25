#ifndef RACKET_AST_HPP
#define RACKET_AST_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace racket {

// Forward declarations
class ASTVisitor;

// ============================================================================
// Base AST Node
// ============================================================================

/**
 * @brief Base class for all AST nodes
 * 
 * All nodes in the Abstract Syntax Tree inherit from this class.
 * Implements the Visitor pattern for tree traversal.
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    
    /**
     * @brief Accept a visitor for tree traversal
     * @param visitor The visitor to accept
     */
    virtual void accept(ASTVisitor& visitor) = 0;
    
    /**
     * @brief Print the AST node (for debugging)
     * @param indent Indentation level
     */
    virtual void print(int indent = 0) const = 0;
    
protected:
    void printIndent(int indent) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};

// ============================================================================
// Expression Nodes
// ============================================================================

/**
 * @brief Base class for all expression nodes
 * 
 * In Racket, everything is an expression that evaluates to a value.
 */
class ExprNode : public ASTNode {
public:
    virtual ~ExprNode() = default;
};

// ============================================================================
// Literal Nodes
// ============================================================================

/**
 * @brief Identifier node (variable reference)
 * 
 * Represents: id
 * Example: x, foo, my-var
 */
class IdNode : public ExprNode {
public:
    std::string name;
    
    explicit IdNode(const std::string& name) : name(name) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief Number literal node
 * 
 * Represents: numeric literals
 * Example: 42, 3.14, -5
 */
class NumberNode : public ExprNode {
public:
    double value;
    
    explicit NumberNode(double value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief String literal node
 * 
 * Represents: string literals
 * Example: "hello", "world"
 */
class StringNode : public ExprNode {
public:
    std::string value;
    
    explicit StringNode(const std::string& value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief Boolean literal node
 * 
 * Represents: #t or #f
 * Example: #t, #f
 */
class BooleanNode : public ExprNode {
public:
    bool value;
    
    explicit BooleanNode(bool value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

// ============================================================================
// Special Form Nodes
// ============================================================================

/**
 * @brief Quote expression node
 * 
 * Represents: (quote datum)
 * Example: (quote x), (quote (1 2 3))
 * 
 * Quote prevents evaluation of its argument.
 */
class QuoteNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> datum;
    
    explicit QuoteNode(std::unique_ptr<ExprNode> datum) 
        : datum(std::move(datum)) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief If conditional expression node
 * 
 * Represents: (if expr expr expr)
 * Example: (if #t 1 0)
 * 
 * Evaluates condition, then either thenBranch or elseBranch.
 */
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

/**
 * @brief Begin sequence expression node
 * 
 * Represents: (begin expr ...+)
 * Example: (begin (print "hi") 42)
 * 
 * Evaluates expressions in sequence, returns last value.
 */
class BeginNode : public ExprNode {
public:
    std::vector<std::unique_ptr<ExprNode>> expressions;
    
    explicit BeginNode(std::vector<std::unique_ptr<ExprNode>> expressions)
        : expressions(std::move(expressions)) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief Function application node
 * 
 * Represents: (#%plain-app expr ...+)
 * Example: (#%plain-app + 1 2)
 * 
 * Applies function to arguments.
 */
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

// ============================================================================
// Visitor Pattern Interface
// ============================================================================

/**
 * @brief Visitor interface for AST traversal
 * 
 * Implements the Visitor pattern to allow operations on the AST
 * without modifying the node classes.
 */
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

} // namespace racket

#endif // RACKET_AST_HPP

// Made with Bob
