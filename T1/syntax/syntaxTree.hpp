#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include <string>
#include <vector>
using namespace std;

class Node {
    protected:
        string value;

    public:
        Node(string c);
        string getValue() const;
        virtual void print(int indent = 0);
        virtual ~Node();
};

class OperativeNodeUnary : public Node {
    Node* operand;

    public:
        OperativeNodeUnary(string c, Node* operand);
        Node* getOperand() const;
        void print(int indent = 0) override;
        ~OperativeNodeUnary();
};

class OperativeNodeBinary : public Node {
    Node* leftOperand;
    Node* rightOperand;

    public:
        OperativeNodeBinary(string c, Node* leftOperand, Node* rightOperand);
        Node* getLeftOperand() const;
        Node* getRightOperand() const;
        void print(int indent = 0) override;
        ~OperativeNodeBinary();
};

class CharacterClassNode : public Node {
    string charClass; 

    public:
        CharacterClassNode(string c);
        string getCharClass() const;
        void print(int indent = 0) override;
        ~CharacterClassNode();
};

class regularExpressionToken {
    string tokenType;

    public:
        Node* createSyntaxTree(string regularExpression);

    private:
        bool isOperator(string c);
        bool IsOperatorUnary(string c);
        string expandCharacterClasses(string regularExpression);
};

vector<string> readRegexFromFile(const string& filename);
void printTree(Node* root);

#endif 