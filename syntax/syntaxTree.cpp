#include "syntaxTree.hpp"
#include <cstring>
#include <stack>
#include <iostream>
#include <fstream>
using namespace std;

// Implementações da classe Node
Node::Node(string c) {
    value = c;
}

string Node::getValue() const {
    return value;
}

void Node::print(int indent) {
    cout << string(indent, ' ') << value << endl;
}

Node::~Node() {}

// Implementações da classe OperativeNodeUnary
OperativeNodeUnary::OperativeNodeUnary(string c, Node* operand) : Node(c) {
    this->operand = operand;
}

Node* OperativeNodeUnary::getOperand() const {
    return operand;
}

void OperativeNodeUnary::print(int indent) {
    cout << string(indent, ' ') << value << endl;
    operand->print(indent + 2);
}

OperativeNodeUnary::~OperativeNodeUnary() {
    delete operand;
}

// Implementações da classe OperativeNodeBinary
OperativeNodeBinary::OperativeNodeBinary(string c, Node* leftOperand, Node* rightOperand) : Node(c) {
    this->leftOperand = leftOperand;
    this->rightOperand = rightOperand;
}

Node* OperativeNodeBinary::getLeftOperand() const {
    return leftOperand;
}

Node* OperativeNodeBinary::getRightOperand() const {
    return rightOperand;
}

void OperativeNodeBinary::print(int indent) {
    cout << string(indent, ' ') << value << endl;
    leftOperand->print(indent + 2);
    rightOperand->print(indent + 2);
}

OperativeNodeBinary::~OperativeNodeBinary() {
    delete leftOperand;
    delete rightOperand;
}

// Implementações da classe regularExpressionToken
Node* regularExpressionToken::createSyntaxTree(string regularExpression){
    // Lógica para criar a árvore de sintaxe a partir da expressão regular
    stack<Node*> pilha;
    // push, top, pop, empty (mostra se ta vazia), size

    string reading = "";
    for (size_t i = 0; i < regularExpression.size(); i++) {
        char currentChar = regularExpression[i];
        if (currentChar == ' ') {
            if (!reading.empty()) {
                if (isOperator(reading)) {
                    if (IsOperatorUnary(reading)) {
                        Node* operand = pilha.top(); 
                        pilha.pop();
                        Node* newNode = new OperativeNodeUnary(reading, operand);
                        pilha.push(newNode);
                    } else {
                        Node* right = pilha.top(); 
                        pilha.pop();
                        Node* left = pilha.top(); 
                        pilha.pop();
                        Node* newNode = new OperativeNodeBinary(reading, left, right);
                        pilha.push(newNode);
                    }
                } else {
                    Node* newNode = new Node(reading);
                    pilha.push(newNode);
                }
                reading = "";
            }
        } else {
            reading += currentChar;
        }
    }
    if (!reading.empty()) {
        if (isOperator(reading)) {
            if (IsOperatorUnary(reading)) {
                Node* operand = pilha.top(); 
                pilha.pop();
                Node* newNode = new OperativeNodeUnary(reading, operand);
                pilha.push(newNode);
            } else {
                Node* right = pilha.top(); 
                pilha.pop();
                Node* left = pilha.top(); 
                pilha.pop();
                Node* newNode = new OperativeNodeBinary(reading, left, right);
                pilha.push(newNode);
            }
        } else {
            Node* newNode = new Node(reading);
            pilha.push(newNode);
        }
    }
    return pilha.top();
}

bool regularExpressionToken::isOperator(string c) {
    // *: zero ou mais ocorrências do elemento anterior
    // +: uma ou mais ocorrências do elemento anterior
    // ?: zero ou uma ocorrência do elemento anterior
    // |: alternância (ou)
    // .: concatenação
    return c == "*" || c == "+" || c == "?" || c == "|" || c == ".";
}

bool regularExpressionToken::IsOperatorUnary(string c) {
    // *, + e ? são operadores unários, ou seja, só tem um operando
    return c == "*" || c == "+" || c == "?";
}

// Função de leitura do arquivo, retorna a lista de linhas do arquivo
vector<string> readRegexFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

void printTree(Node* root) {
    if (root) {
        root->print(0);
    }
    printf("\n");
}