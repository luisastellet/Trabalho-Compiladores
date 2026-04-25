#include "racketAST.hpp"
#include <iostream>

namespace racket {

// ============================================================================
// IdNode Implementation
// ============================================================================

void IdNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void IdNode::print(int indent) const {
    printIndent(indent);
    std::cout << "IdNode: " << name << std::endl;
}

// ============================================================================
// NumberNode Implementation
// ============================================================================

void NumberNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void NumberNode::print(int indent) const {
    printIndent(indent);
    std::cout << "NumberNode: " << value << std::endl;
}

// ============================================================================
// StringNode Implementation
// ============================================================================

void StringNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void StringNode::print(int indent) const {
    printIndent(indent);
    std::cout << "StringNode: \"" << value << "\"" << std::endl;
}

// ============================================================================
// BooleanNode Implementation
// ============================================================================

void BooleanNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void BooleanNode::print(int indent) const {
    printIndent(indent);
    std::cout << "BooleanNode: " << (value ? "#t" : "#f") << std::endl;
}

// ============================================================================
// QuoteNode Implementation
// ============================================================================

void QuoteNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void QuoteNode::print(int indent) const {
    printIndent(indent);
    std::cout << "QuoteNode:" << std::endl;
    if (datum) {
        datum->print(indent + 1);
    }
}

// ============================================================================
// IfNode Implementation
// ============================================================================

void IfNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void IfNode::print(int indent) const {
    printIndent(indent);
    std::cout << "IfNode:" << std::endl;
    
    printIndent(indent + 1);
    std::cout << "condition:" << std::endl;
    if (condition) {
        condition->print(indent + 2);
    }
    
    printIndent(indent + 1);
    std::cout << "then:" << std::endl;
    if (thenBranch) {
        thenBranch->print(indent + 2);
    }
    
    printIndent(indent + 1);
    std::cout << "else:" << std::endl;
    if (elseBranch) {
        elseBranch->print(indent + 2);
    }
}

// ============================================================================
// BeginNode Implementation
// ============================================================================

void BeginNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void BeginNode::print(int indent) const {
    printIndent(indent);
    std::cout << "BeginNode: (" << expressions.size() << " expressions)" << std::endl;
    
    for (size_t i = 0; i < expressions.size(); ++i) {
        printIndent(indent + 1);
        std::cout << "[" << i << "]:" << std::endl;
        if (expressions[i]) {
            expressions[i]->print(indent + 2);
        }
    }
}

// ============================================================================
// AppNode Implementation
// ============================================================================

void AppNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void AppNode::print(int indent) const {
    printIndent(indent);
    std::cout << "AppNode:" << std::endl;
    
    printIndent(indent + 1);
    std::cout << "function:" << std::endl;
    if (function) {
        function->print(indent + 2);
    }
    
    printIndent(indent + 1);
    std::cout << "arguments: (" << arguments.size() << ")" << std::endl;
    for (size_t i = 0; i < arguments.size(); ++i) {
        printIndent(indent + 2);
        std::cout << "[" << i << "]:" << std::endl;
        if (arguments[i]) {
            arguments[i]->print(indent + 3);
        }
    }
}

} // namespace racket

// Made with Bob
