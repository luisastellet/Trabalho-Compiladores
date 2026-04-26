#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Token {
	int type;        // ID do token (-1 = erro)
	char value[256]; // Valor do token
};

int isSeparator(char c) {
	return c == '(' || c == ')' || c == '{' || c == '}' ||
	       c == ';' || c == ',' || c == '=';
}

// Função helper para comparar símbolo no input
int matchSymbol(const char* input, int pos, const char* symbol) {
	size_t len = strlen(symbol);
	if (strncmp(&input[pos], symbol, len) == 0) return len;
	return 0;
}

// Extrai o próximo lexema (sequência completa de caracteres não-separadores)
// Se começar com aspas, extrai até a aspas de fechamento
int extractLexeme(const char* input, int pos, char* lexeme) {
	int len = 0;
	// Se começar com aspas, extrair string entre aspas
	if (input[pos] == '\"') {
		if (len < 255) lexeme[len++] = input[pos++];
		while (input[pos] != '\0' && input[pos] != '\"') {
			if (len < 255) lexeme[len++] = input[pos++];
		}
		if (input[pos] == '\"') {
			if (len < 255) lexeme[len++] = input[pos++];
		}
	} else {
		// Caso normal: extrair até separador ou espaço
		while (input[pos] != '\0' && !isspace(input[pos]) && !isSeparator(input[pos])) {
			if (len < 255) lexeme[len++] = input[pos++];
		}
	}
	lexeme[len] = '\0';
	return len;
}

struct Token validateLexeme(const char* lexeme) {
	struct Token token;
	token.type = -1;
	memset(token.value, 0, sizeof(token.value));
	strcpy(token.value, lexeme);

	int currentState = 0;
	int lastFinalState = -1;
	int lastFinalPos = 0;
	int pos = 0;

	// Tentar avançar através do DFA com o lexema completo
	while (lexeme[pos] != '\0') {
		int nextState = -1;
		int symbolLen = 0;

		switch (currentState) {
			case 0: {
				if (lexeme[pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '7') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '(') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (lexeme[pos] == ')') {
					nextState = 3;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '0') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '1') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '2') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '3') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '4') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '5') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '6') {
					nextState = 4;
					symbolLen = 1;
				}
 else if ((symbolLen = matchSymbol(lexeme, pos, "\""))) {
					nextState = 1;
				}
 else if (lexeme[pos] == '8') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '9') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '[') {
					nextState = 5;
					symbolLen = 1;
				}
 else if (lexeme[pos] == ']') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 8;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 1: {
				if (lexeme[pos] == 'm') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'n') {
					nextState = 1;
					symbolLen = 1;
				}
 else if ((symbolLen = matchSymbol(lexeme, pos, "\""))) {
					nextState = 11;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 1;
					symbolLen = 1;
				}
				break;
			}

			case 2: {
				break;
			}

			case 3: {
				break;
			}

			case 4: {
				if (lexeme[pos] == '0') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '1') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '2') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '3') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '4') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '5') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '6') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '7') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '8') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (lexeme[pos] == '9') {
					nextState = 4;
					symbolLen = 1;
				}
				break;
			}

			case 5: {
				break;
			}

			case 6: {
				break;
			}

			case 7: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 8: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 12;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 9: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 13;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 10: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 14;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 11: {
				break;
			}

			case 12: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 15;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 13: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 14: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 15: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 17;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 16: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 18;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 17: {
				if (lexeme[pos] == 'n') {
					nextState = 19;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 18: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 20;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 19: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 20: {
				if (lexeme[pos] == 'n') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'z') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'y') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'x') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'w') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'v') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'u') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 't') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 's') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'r') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'q') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'p') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'o') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'a') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'm') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'l') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'k') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'j') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'i') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'h') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'g') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'f') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'e') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'c') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (lexeme[pos] == 'b') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			default:
				nextState = -1;
				break;
		}

		if (nextState == -1) break;

		currentState = nextState;
		pos += symbolLen;

		// Verificar se é estado final (longest match)
		switch (currentState) {
			case 2:
				lastFinalState = 0;
				lastFinalPos = pos;
				break;
			case 3:
				lastFinalState = 1;
				lastFinalPos = pos;
				break;
			case 4:
				lastFinalState = 7;
				lastFinalPos = pos;
				break;
			case 5:
				lastFinalState = 2;
				lastFinalPos = pos;
				break;
			case 6:
				lastFinalState = 3;
				lastFinalPos = pos;
				break;
			case 7:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 8:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 9:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 10:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 11:
				lastFinalState = 8;
				lastFinalPos = pos;
				break;
			case 12:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 13:
				lastFinalState = 4;
				lastFinalPos = pos;
				break;
			case 14:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 15:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 16:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 17:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 18:
				lastFinalState = 9;
				lastFinalPos = pos;
				break;
			case 19:
				lastFinalState = 5;
				lastFinalPos = pos;
				break;
			case 20:
				lastFinalState = 6;
				lastFinalPos = pos;
				break;
			default:
				break;
		}
	}

	// Retornar ao último estado final (longest match)
	// MAS APENAS SE CONSUMIU TODO O LEXEMA
	if (lastFinalState != -1 && lastFinalPos == strlen(lexeme)) {
		token.type = lastFinalState;
	} else {
		token.type = -1;  // Lexema não corresponde a nenhum token
	}

	return token;
}

int main(int argc, char* argv[]) {
	const char* tokenNames[] = {
		"TOKEN_LPAREN",
		"TOKEN_RPAREN",
		"TOKEN_LBRACKET",
		"TOKEN_RBRACKET",
		"TOKEN_IF",
		"TOKEN_BEGIN",
		"TOKEN_QUOTE",
		"TOKEN_NUMBER",
		"TOKEN_STRING",
		"TOKEN_IDENTIFIER"
	};

	// Verificar argumentos
	const char* inputFile = (argc > 1) ? argv[1] : "tests/test_input.txt";
	const char* outputFile = "tokens.txt";

	FILE* file = fopen(inputFile, "r");
	if (!file) {
		fprintf(stderr, "Erro ao abrir arquivo: %s\n", inputFile);
		return 1;
	}

	FILE* tokensFile = fopen(outputFile, "w");
	if (!tokensFile) {
		fprintf(stderr, "Erro ao criar arquivo: %s\n", outputFile);
		fclose(file);
		return 1;
	}

	printf("Scanner Léxico Gerado\n");
	printf("═════════════════════════════════════\n\n");

	printf("Tokens definidos:\n");
	printf("  Token %zu: %s\n", (size_t)0, tokenNames[0]);
	printf("  Token %zu: %s\n", (size_t)1, tokenNames[1]);
	printf("  Token %zu: %s\n", (size_t)2, tokenNames[2]);
	printf("  Token %zu: %s\n", (size_t)3, tokenNames[3]);
	printf("  Token %zu: %s\n", (size_t)4, tokenNames[4]);
	printf("  Token %zu: %s\n", (size_t)5, tokenNames[5]);
	printf("  Token %zu: %s\n", (size_t)6, tokenNames[6]);
	printf("  Token %zu: %s\n", (size_t)7, tokenNames[7]);
	printf("  Token %zu: %s\n", (size_t)8, tokenNames[8]);
	printf("  Token %zu: %s\n", (size_t)9, tokenNames[9]);

	printf("Testes:\n");
	printf("─────────────────────────────────────\n");

	char line[512];

	while (fgets(line, sizeof(line), file)) {
		// Remover quebra de linha
		size_t len = strlen(line);
		if (len > 0 && line[len-1] == '\n') {
			line[len-1] = '\0';
		}
		if (len > 0 && line[len-1] == '\r') {
			line[len-1] = '\0';
		}
		// Ignorar linhas vazias
		if (strlen(line) == 0) continue;

		printf("Entrada: %s\n", line);

		int pos = 0;
		while (line[pos] != '\0') {
			// Ignorar espaços em branco
			while (line[pos] != '\0' && isspace(line[pos])) {
				pos++;
			}
			if (line[pos] == '\0') break;

			// Fase 1: Separadores são tratados imediatamente
			if (isSeparator(line[pos])) {
				char sep[2] = {line[pos], '\0'};
				struct Token token = validateLexeme(sep);
				if (token.type >= 0) {
					// Escrever no arquivo tokens.txt
					fprintf(tokensFile, "%d %s\n", token.type, token.value);
					// Imprimir na tela com tratamento de caracteres não-imprimíveis
					printf("  -> %s ('", tokenNames[token.type]);
					if (line[pos] >= 32 && line[pos] < 127) {
						printf("%c", line[pos]);
					} else {
						printf("[%d]", (int)(unsigned char)line[pos]);
					}
					printf("')\n");
				} else {
					printf("  -> Não reconhecido: '%c'\n", line[pos]);
				}
				pos++;
			} else {
				// Fase 2: Extrair lexema completo (sequência de caracteres não-separadores)
				char lexeme[256];
				int lexemeLen = extractLexeme(line, pos, lexeme);
				if (lexemeLen > 0) {
					// Validar o lexema completo contra todos os tokens
					struct Token token = validateLexeme(lexeme);
					if (token.type >= 0) {
						// Escrever no arquivo tokens.txt
						fprintf(tokensFile, "%d %s\n", token.type, token.value);
						// Imprimir na tela
						printf("  -> %s ('%s')\n", tokenNames[token.type], token.value);
					} else {
						printf("  -> Erro: '%s' não corresponde a nenhum token\n", lexeme);
					}
					pos += lexemeLen;
				} else {
					pos++; // Avança se não conseguiu extrair
				}
			}
		}
		printf("\n");
	}

	fclose(file);
	fclose(tokensFile);
	printf("\n");
	printf("Tokens salvos em: %s\n", outputFile);
	return 0;
}
