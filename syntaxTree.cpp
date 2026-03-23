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
        // TODO: Não podemos ler char a char, precisamos ler token a token, ou seja, ler até o próximo espaço

        string reading = "";
        for (size_t i = 0; i < regularExpression.size(); i++) {
            char c = regularExpression[i];

            // Se achar espaço em branco, o que leu antes é um tokem
            if (c == ' ') {
                
                // Se é operador
                if (isOperator(reading)) {
                    // Se for um operando unário
                    if (IsOperatorUnary(reading)) {
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
                reading += c;
            }
        }
    }

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

// Nós da árvore só com valor
class Node {
    string value;

    public:
    Node(string c) {
        value = c;
    }
};

class OperativeNodeUnary : public Node {
    Node* operand;

    public:
    OperativeNodeUnary(string c, Node* operand) : Node(c) {
        this->operand = operand;
    }
};

class OperativeNodeBinary : public Node {
    Node* left;
    Node* right;

    public:
    OperativeNodeBinary(string c, Node* left, Node* right) : Node(c) {
        this->left = left;
        this->right = right;
    }
};


// TODOZÃO: 
// - Tratar a leitura da expressaão regular com os espaços em branco
// - Trocar a variável 'c' nos métodos
// - Separar as classes em arquivos distintos
// - Olhar restrição de acesso
// - Ajeitar para não precisar terminar em espaço em branco as entradas
