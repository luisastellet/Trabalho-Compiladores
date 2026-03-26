#include <cstring>
#include <string>
#include <stack>
using namespace std;

class regularExpressionToken{
    string tokenType;
    Node *tokenExpression;

    Node* createSyntaxTree(string regularExpression){
        // Lógica para criar a árvore de sintaxe a partir da expressão regular
        stack<Node*> pilha;
        // push, top, pop, empty (mostra se ta vazia), size
 
        string reading = "";
        for (size_t i = 0; i < regularExpression.size(); i++) {
            char currentChar = regularExpression[i];

            // Se achar espaço em branco ou o final da expressão regular, o que leu antes é um tokem
            if (currentChar == ' ' || i == regularExpression.size() - 1) {
                
                // Se é operador
                if (isOperator(reading.back())) {
                    // Se for um operando unário
                    if (IsOperatorUnary(reading.back())) {
                        Node* operand = pilha.top(); 
                        pilha.pop();
                        Node* newNode = new OperativeNodeUnary(reading, operand);
                        pilha.push(newNode);
                    // Se for um operando binário
                    } else {
                        Node* right = pilha.top(); 
                        pilha.pop();
                        Node* left = pilha.top(); 
                        pilha.pop();
                        Node* newNode = new OperativeNodeBinary(reading, left, right);
                        pilha.push(newNode);
                    }
                }
                // Se não for um operador
                else {
                    Node* newNode = new Node(reading);
                    pilha.push(newNode);
                }
                reading = "";
            }
            else {
                reading += currentChar;
            }
        }
    }

    bool isOperator(char c) {
        // *: zero ou mais ocorrências do elemento anterior
        // +: uma ou mais ocorrências do elemento anterior
        // ?: zero ou uma ocorrência do elemento anterior
        // |: alternância (ou)
        // .: concatenação
        return c == '*' || c == '+' || c == '?' || c == '|' || c == '.';
    }

    bool IsOperatorUnary(char c) {
        // *, + e ? são operadores unários, ou seja, só tem um operando
        return c == '*' || c == '+' || c == '?';
    }
};

// Nós da árvore só com valor
class Node {
    string value;

    public:
    Node(string c) {
        value = c;
    }
};

// Nós da árvore que representam operadores unários, ou seja, tem um ramo
class OperativeNodeUnary : public Node {
    Node* operand;

    public:
    OperativeNodeUnary(string c, Node* operand) : Node(c) {
        this->operand = operand;
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
};