#include "../racket/scannerAdapter.hpp"
#include "../racket/racketParser.hpp"
#include "../racket/racketAST.hpp"
#include "../racket/firstFollow.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace racket;

/**
 * @brief Ler todo o arquivo para uma string
 */
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Testar o lexer lendo tokens de tokens.txt
 */
void testLexer(const std::string& tokensFile) {
    std::cout << "\n- TESTE DO LEXER\n" << std::endl;
    std::cout << "Arquivo de tokens: " << tokensFile << std::endl;
    std::cout << std::endl;
    
    ScannerAdapter scanner(tokensFile);
    std::vector<Token> tokens = scanner.tokenizeAll();
    
    std::cout << "Tokens (" << tokens.size() << "):" << std::endl;
    for (const auto& token : tokens) {
        token.print();
    }
    std::cout << std::endl;
}

/**
 * @brief Testar o parser lendo tokens de tokens.txt
 */
void testParser(const std::string& tokensFile) {
    std::cout << "\n- TESTE DO PARSER\n" << std::endl;
    std::cout << "Arquivo de tokens: " << tokensFile << std::endl;
    std::cout << std::endl;
    
    try {
        ScannerAdapter scanner(tokensFile);
        RacketParser parser(scanner);
        
        auto ast = parser.parse();
        
        std::cout << "AST:" << std::endl;
        ast->print();
        std::cout << std::endl;
        
        std::cout << "✓ Parse bem-sucedido!" << std::endl;
    } catch (const ParseError& e) {
        std::cerr << "✗ " << e.getFormattedMessage() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "✗ Erro: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Testar o parser lendo múltiplas expressões de tokens.txt
 */
void testParserMultiple(const std::string& tokensFile) {
    std::cout << "\n- TESTE DO PARSER MULTIPLO\n" << std::endl;
    std::cout << "Arquivo de tokens: " << tokensFile << std::endl;
    std::cout << std::endl;
    
    try {
        ScannerAdapter scanner(tokensFile);
        RacketParser parser(scanner);
        
        auto expressions = parser.parseMultiple();
        
        std::cout << "Expressoes analisadas: " << expressions.size() << std::endl;
        std::cout << std::endl;
        
        for (size_t i = 0; i < expressions.size(); ++i) {
            std::cout << "Expressao " << (i + 1) << ":" << std::endl;
            expressions[i]->print(1);
            std::cout << std::endl;
        }
        
        std::cout << "✓ Todas as expressoes foram analisadas com sucesso!" << std::endl;
    } catch (const ParseError& e) {
        std::cerr << "✗ " << e.getFormattedMessage() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "✗ Erro: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Executar REPL interativo (não suportado com o fluxo tokens.txt)
 */
void runREPL() {
    std::cerr << "Erro: REPL interativo não suportado com o fluxo tokens.txt" << std::endl;
    std::cerr << "Use o fluxo scanner → tokens.txt → parser" << std::endl;
}

/**
 * @brief Mostrar conjuntos FIRST e FOLLOW
 */
void showFirstFollow() {
    std::cout << "\n- ANALISE DE FIRST E FOLLOW\n" << std::endl;
    std::cout << "Calculando FIRST e FOLLOW" << std::endl;
    std::cout << std::endl;
    
    FirstFollowCalculator calculator;
    
    // Calcular conjuntos FIRST
    calculator.calculateFirst();
    calculator.printFirst();
    
    // Calcular conjuntos FOLLOW
    calculator.calculateFollow();
    calculator.printFollow();
    
    // Verificar LL(1)
    calculator.isLL1();
}

/**
 * @brief Imprimir informações de uso
 */
void printUsage(const char* programName) {
    std::cout << "Uso: " << programName << " [opções] [arquivo]" << std::endl;
    std::cout << std::endl;
    std::cout << "Opções:" << std::endl;
    std::cout << "  -h, --help       Mostrar esta mensagem de ajuda" << std::endl;
    std::cout << "  -l, --lex        Executar apenas o teste do lexer" << std::endl;
    std::cout << "  -p, --parse      Executar o teste do parser (padrão)" << std::endl;
    std::cout << "  -m, --multiple   Analisar múltiplas expressões a partir de um arquivo" << std::endl;
    std::cout << "  -f, --first-follow  Mostrar conjuntos FIRST e FOLLOW" << std::endl;
    std::cout << std::endl;
    std::cout << "Exemplos:" << std::endl;
    std::cout << "  " << programName << " tokens.txt" << std::endl;
    std::cout << "  " << programName << " -l tokens.txt" << std::endl;
    std::cout << "  " << programName << " -m tokens.txt" << std::endl;
    std::cout << "  " << programName << " -f" << std::endl;
}

/**
 * @brief Ponto de entrada principal

 */
int main(int argc, char* argv[]) {
    bool lexOnly = false;
    bool parseMultiple = false;
    bool interactive = false;
    bool showFirstFollowSets = false;
    std::string filename;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-l" || arg == "--lex") {
            lexOnly = true;
        } else if (arg == "-p" || arg == "--parse") {
            // Modo de análise única (comportamento padrão)
        } else if (arg == "-m" || arg == "--multiple") {
            parseMultiple = true;
        } else if (arg == "-f" || arg == "--first-follow") {
            showFirstFollowSets = true;
        } else if (arg == "-i" || arg == "--interactive") {
            interactive = true;
        } else if (arg[0] != '-') {
            filename = arg;
        } else {
            std::cerr << "Opção desconhecida: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Mostrar conjuntos FIRST e FOLLOW
    if (showFirstFollowSets) {
        showFirstFollow();
        return 0;
    }
    
    if (interactive) {
        runREPL();
        return 0;
    }
    
    std::string tokensFile = filename.empty() ? "tokens.txt" : filename;
    
    try {
        if (lexOnly) {
            testLexer(tokensFile);
        } else if (parseMultiple) {
            testParserMultiple(tokensFile);
        } else {
            testParser(tokensFile);
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Erro fatal: " << e.what() << std::endl;
        return 1;
    }
}
