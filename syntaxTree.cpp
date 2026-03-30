#include <cstring>
#include <string>
#include <stack>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// Nós da árvore só com valor
class Node {
protected:
    string value;

public:
    Node(string c) {
        value = c;
    }

    virtual void print(int indent = 0) {
        cout << string(indent, ' ') << value << endl;
    }

    virtual ~Node() {}
};

// Nós da árvore que representam operadores unários, ou seja, tem um ramo
class OperativeNodeUnary : public Node {
    Node* operand;

    public:
    OperativeNodeUnary(string c, Node* operand) : Node(c) {
        this->operand = operand;
    }

    void print(int indent = 0) override {
        cout << string(indent, ' ') << value << endl;
        operand->print(indent + 2);
    }

    ~OperativeNodeUnary() {
        delete operand;
    }
};

// Nós da árvore que representam operadores binários, ou seja, tem dois ramos
class OperativeNodeBinary : public Node {
    Node* leftOperand;
    Node* rightOperand;

    public:
    OperativeNodeBinary(string c, Node* leftOperand, Node* rightOperand) : Node(c) {
        this->leftOperand = leftOperand;
        this->rightOperand = rightOperand;
    }

    void print(int indent = 0) override {
        cout << string(indent, ' ') << value << endl;
        leftOperand->print(indent + 2);
        rightOperand->print(indent + 2);
    }

    ~OperativeNodeBinary() {
        delete leftOperand;
        delete rightOperand;
    }
};

class regularExpressionToken{
    string tokenType;
    Node *tokenExpression;

public:
    Node* createSyntaxTree(string regularExpression){
        // Lógica para criar a árvore de sintaxe a partir da expressão regular
        stack<Node*> pilha;
        // push, top, pop, empty (mostra se ta vazia), size
 
        string reading = "";
        for (size_t i = 0; i < regularExpression.size(); i++) {
            char currentChar = regularExpression[i];
            if (currentChar == ' ') {
                if (!reading.empty()) {
                    // process reading
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

private:
    bool isOperator(string c) {
        // *: zero ou mais ocorrências do elemento anterior
        // +: uma ou mais ocorrências do elemento anterior
        // ?: zero ou uma ocorrência do elemento anterior
        // |: alternância (ou)
        // .: concatenação
        return c == "*" || c == "+" || c == "?" || c == "|" || c == ".";
    }

    bool IsOperatorUnary(string c) {
        // *, + e ? são operadores unários, ou seja, só tem um operando
        return c == "*" || c == "+" || c == "?";
    }
};

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
}

// Apagar depois, só para teste
int main() {
    regularExpressionToken ret;
    vector<string> lines = readRegexFromFile("input.txt");
    if (lines.empty()) {
        cout << "Erro ao ler input.txt" << endl;
        return 1;
    }

    string regex;
    for (const auto& line : lines) {
        cout << "Lendo linha: " << line << endl;
        regex = line;
        Node* tree = ret.createSyntaxTree(regex);
        printTree(tree);
        delete tree;
    }
    
    return 0;
}