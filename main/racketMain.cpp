#include "../racket/scannerAdapter.hpp"
#include "../racket/racketParser.hpp"
#include "../racket/racketAST.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace racket;

/**
 * @brief Read entire file into a string
 */
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Test the lexer by reading tokens from tokens.txt
 */
void testLexer(const std::string& tokensFile) {
    std::cout << "=== LEXER TEST ===" << std::endl;
    std::cout << "Using: Generated Scanner (via tokens.txt)" << std::endl;
    std::cout << "Tokens file: " << tokensFile << std::endl;
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
 * @brief Test the parser by parsing tokens from tokens.txt
 */
void testParser(const std::string& tokensFile) {
    std::cout << "=== PARSER TEST ===" << std::endl;
    std::cout << "Using: Generated Scanner (via tokens.txt)" << std::endl;
    std::cout << "Tokens file: " << tokensFile << std::endl;
    std::cout << std::endl;
    
    try {
        ScannerAdapter scanner(tokensFile);
        RacketParser parser(scanner);
        
        auto ast = parser.parse();
        
        std::cout << "AST:" << std::endl;
        ast->print();
        std::cout << std::endl;
        
        std::cout << "✓ Parse successful!" << std::endl;
    } catch (const ParseError& e) {
        std::cerr << "✗ " << e.getFormattedMessage() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "✗ Error: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Test parsing multiple expressions from tokens.txt
 */
void testParserMultiple(const std::string& tokensFile) {
    std::cout << "=== PARSER MULTIPLE TEST ===" << std::endl;
    std::cout << "Using: Generated Scanner (via tokens.txt)" << std::endl;
    std::cout << "Tokens file: " << tokensFile << std::endl;
    std::cout << std::endl;
    
    try {
        ScannerAdapter scanner(tokensFile);
        RacketParser parser(scanner);
        
        auto expressions = parser.parseMultiple();
        
        std::cout << "Parsed " << expressions.size() << " expressions:" << std::endl;
        std::cout << std::endl;
        
        for (size_t i = 0; i < expressions.size(); ++i) {
            std::cout << "Expression " << (i + 1) << ":" << std::endl;
            expressions[i]->print(1);
            std::cout << std::endl;
        }
        
        std::cout << "✓ All expressions parsed successfully!" << std::endl;
    } catch (const ParseError& e) {
        std::cerr << "✗ " << e.getFormattedMessage() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "✗ Error: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Run interactive REPL (not supported with tokens.txt flow)
 */
void runREPL() {
    std::cerr << "Error: Interactive REPL not supported with tokens.txt flow" << std::endl;
    std::cerr << "Please use the scanner → tokens.txt → parser flow instead" << std::endl;
}

/**
 * @brief Print usage information
 */
void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] [file]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help       Show this help message" << std::endl;
    std::cout << "  -l, --lex        Run lexer test only" << std::endl;
    std::cout << "  -p, --parse      Run parser test (default)" << std::endl;
    std::cout << "  -m, --multiple   Parse multiple expressions from file" << std::endl;
    std::cout << "  -i, --interactive Run interactive REPL" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << " tokens.txt" << std::endl;
    std::cout << "  " << programName << " -l tokens.txt" << std::endl;
    std::cout << "  " << programName << " -m tokens.txt" << std::endl;
}

/**
 * @brief Main entry point
 */
int main(int argc, char* argv[]) {
    // Parse command line arguments
    bool lexOnly = false;
    bool parseMultiple = false;
    bool interactive = false;
    std::string filename;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-l" || arg == "--lex") {
            lexOnly = true;
        } else if (arg == "-p" || arg == "--parse") {
            // Single parse mode (default behavior)
        } else if (arg == "-m" || arg == "--multiple") {
            parseMultiple = true;
        } else if (arg == "-i" || arg == "--interactive") {
            interactive = true;
        } else if (arg[0] != '-') {
            filename = arg;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Interactive mode
    if (interactive) {
        runREPL();
        return 0;
    }
    
    // Use tokens.txt as default if no file specified
    std::string tokensFile = filename.empty() ? "tokens.txt" : filename;
    
    try {
        if (lexOnly) {
            testLexer(tokensFile);
        } else if (parseMultiple) {
            testParserMultiple(tokensFile);
        } else {
            // Default: parse single expression
            testParser(tokensFile);
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}

// Made with Bob
