#ifndef RACKET_AST_HPP
#define RACKET_AST_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace racket {

// Declarações antecipadas
class ASTVisitor;

// Nó base da AST

/**
 * @brief Nó base para todos os nós da AST
 * 
 * Todos os nós na Árvore de Sintaxe Abstrata herdam desta classe.
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    
    /**
     * @brief Aceita um visitante para a travessia da árvore
     * @param visitor O visitante a aceitar
     */
    virtual void accept(ASTVisitor& visitor) = 0;
    
    /**
     * @brief Imprime o nó da AST
     * @param indent Nível de indentação
     */
    virtual void print(int indent = 0) const = 0;
    
protected:
    void printIndent(int indent) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};

// Nós de Expressão

/**
 * @brief Nó base para todos os nós de expressão
 */
class ExprNode : public ASTNode {
public:
    virtual ~ExprNode() = default;
};

// Nós Literais

/**
 * @brief Nó de identificador
 * 
 * Representa: id
 * Exemplo: x, foo, my-var
 */
class IdNode : public ExprNode {
public:
    std::string name;
    
    explicit IdNode(const std::string& name) : name(name) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief Nó literal numérico
 * 
 * Representa: literais numéricos
 * Exemplo: 42, 3.14, -5
 */
class NumberNode : public ExprNode {
public:
    double value;
    
    explicit NumberNode(double value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief Nó literal de string
 * 
 * Representa: literais de string
 * Exemplo: "hello", "world"
 */
class StringNode : public ExprNode {
public:
    std::string value;
    
    explicit StringNode(const std::string& value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

/**
 * @brief Nó literal booleano
 * 
 * Representa: #t ou #f
 * Exemplo: #t, #f
 */
class BooleanNode : public ExprNode {
public:
    bool value;
    
    explicit BooleanNode(bool value) : value(value) {}
    
    void accept(ASTVisitor& visitor) override;
    void print(int indent = 0) const override;
};

// Nós de formas especiais

/**
 * @brief Nó de expressão quote
 * 
 * Representa: (quote datum)
 * Exemplo: (quote x), (quote (1 2 3))
 * 
 * Quote impede a avaliação de seu argumento.
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
 * @brief Nó de expressão condicional if
 * 
 * Representa: (if expr expr expr)
 * Exemplo: (if #t 1 0)
 * 
 * Avalia a condição, depois executa thenBranch ou elseBranch.
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
 * @brief Nó de expressão de sequência begin
 * 
 * Representa: (begin expr ...+)
 * Exemplo: (begin (print "hi") 42)
 * 
 * Avalia expressões em sequência, retorna o último valor.
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
 * @brief Nó de aplicação de função
 * 
 * Representa: (#%plain-app expr ...+)
 * Exemplo: (#%plain-app + 1 2)
 * 
 * Aplica uma função a seus argumentos.
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

// Interface do Padrão Visitor

/**
 * @brief Interface Visitor para percorrer a AST
 * 
 * Implementa o padrão Visitor para permitir operações na AST
 * sem modificar as classes de nó.
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

} 
#endif 