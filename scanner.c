#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Token {
	int type;        // ID do token (-1 = erro)
	char value[256]; // Valor do token
};

// Função helper para comparar símbolo no input
int matchSymbol(const char* input, int pos, const char* symbol) {
	size_t len = strlen(symbol);
	if (strncmp(&input[pos], symbol, len) == 0) return len;
	return 0;
}

struct Token scanToken(const char* input, int* pos) {
	struct Token token;
	token.type = -1;
	memset(token.value, 0, sizeof(token.value));

	int currentState = 0;
	int lastFinalState = -1;
	int lastFinalPos = *pos;
	int tokenStart = *pos;

	while (input[*pos] != '\0') {
		int nextState = -1;
		int symbolLen = 0;

		switch (currentState) {
			case 0: {
				if ((symbolLen = matchSymbol(input, *pos, "\""))) {
					nextState = 1;
				}
 else if (input[*pos] == '(') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == ')') {
					nextState = 3;
					symbolLen = 1;
				}
 else if (input[*pos] == ',') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (input[*pos] == '0') {
					nextState = 5;
					symbolLen = 1;
				}
 else if (input[*pos] == '1') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '2') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '3') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '4') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '5') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '6') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '7') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '8') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == '9') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == ';') {
					nextState = 7;
					symbolLen = 1;
				}
 else if (input[*pos] == '=') {
					nextState = 8;
					symbolLen = 1;
				}
 else if (input[*pos] == 'A') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'B') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'C') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'D') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'E') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'F') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'G') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'H') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'I') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'J') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'K') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'L') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'M') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'N') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'O') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'P') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'Q') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'R') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'S') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'T') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'U') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'V') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'W') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'X') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'Y') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'Z') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 11;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 12;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == '{') {
					nextState = 13;
					symbolLen = 1;
				}
 else if (input[*pos] == '}') {
					nextState = 14;
					symbolLen = 1;
				}
				break;
			}

			case 1: {
				if ((symbolLen = matchSymbol(input, *pos, "\""))) {
					nextState = 15;
				}
 else if (input[*pos] == 'a') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
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
				break;
			}

			case 5: {
				break;
			}

			case 6: {
				if (input[*pos] == '0') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '1') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '2') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '3') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '4') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '5') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '6') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '7') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '8') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '9') {
					nextState = 16;
					symbolLen = 1;
				}
				break;
			}

			case 7: {
				break;
			}

			case 8: {
				break;
			}

			case 9: {
				break;
			}

			case 10: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
					symbolLen = 1;
				}
				break;
			}

			case 11: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 17;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
					symbolLen = 1;
				}
				break;
			}

			case 12: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 18;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
					symbolLen = 1;
				}
				break;
			}

			case 13: {
				break;
			}

			case 14: {
				break;
			}

			case 15: {
				break;
			}

			case 16: {
				if (input[*pos] == '0') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '1') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '2') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '3') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '4') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '5') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '6') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '7') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '8') {
					nextState = 16;
					symbolLen = 1;
				}
 else if (input[*pos] == '9') {
					nextState = 16;
					symbolLen = 1;
				}
				break;
			}

			case 17: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 19;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
					symbolLen = 1;
				}
				break;
			}

			case 18: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
					symbolLen = 1;
				}
				break;
			}

			case 19: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 20;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
					symbolLen = 1;
				}
				break;
			}

			case 20: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'b') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'e') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'f') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'g') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'h') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'i') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'j') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'k') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'l') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'm') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'n') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'o') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'p') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'q') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'r') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 's') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 't') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'u') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'v') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'w') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 10;
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
		// Adicionar símbolo ao valor do token
		for (int i = 0; i < symbolLen; i++) {
			if (*pos - tokenStart + i < 255) {
				token.value[*pos - tokenStart + i] = input[*pos + i];
			}
		}
		*pos += symbolLen;

		// Verificar se é estado final (longest match)
		switch (currentState) {
			case 2:
				lastFinalState = 7;
				lastFinalPos = *pos;
				break;
			case 3:
				lastFinalState = 8;
				lastFinalPos = *pos;
				break;
			case 4:
				lastFinalState = 12;
				lastFinalPos = *pos;
				break;
			case 5:
				lastFinalState = 6;
				lastFinalPos = *pos;
				break;
			case 6:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			case 7:
				lastFinalState = 11;
				lastFinalPos = *pos;
				break;
			case 8:
				lastFinalState = 13;
				lastFinalPos = *pos;
				break;
			case 9:
				lastFinalState = 5;
				lastFinalPos = *pos;
				break;
			case 10:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 11:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 12:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 13:
				lastFinalState = 9;
				lastFinalPos = *pos;
				break;
			case 14:
				lastFinalState = 10;
				lastFinalPos = *pos;
				break;
			case 15:
				lastFinalState = 3;
				lastFinalPos = *pos;
				break;
			case 16:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			case 17:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 18:
				lastFinalState = 0;
				lastFinalPos = *pos;
				break;
			case 19:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 20:
				lastFinalState = 1;
				lastFinalPos = *pos;
				break;
			default:
				break;
		}
	}

	// Retornar ao último estado final (longest match)
	if (lastFinalState != -1) {
		token.type = lastFinalState;
		*pos = lastFinalPos;
		token.value[lastFinalPos - tokenStart] = '\0';
	}

	return token;
}

int main() {
	const char* tokenNames[] = {
		"TOKEN_IF",
		"TOKEN_ELSE",
		"TOKEN_INT",
		"TOKEN_STRING",
		"TOKEN_ID",
		"TOKEN_LETTER",
		"TOKEN_DIGIT",
		"TOKEN_LPAREN",
		"TOKEN_RPAREN",
		"TOKEN_LBRACE",
		"TOKEN_RBRACE",
		"TOKEN_SEMICOLON",
		"TOKEN_COMMA",
		"TOKEN_EQUAL"
	};

	FILE* file = fopen("tests/test_input.txt", "r");
	if (!file) {
		perror("Erro ao abrir tests/test_input.txt");
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
	printf("  Token %zu: %s\n", (size_t)10, tokenNames[10]);
	printf("  Token %zu: %s\n", (size_t)11, tokenNames[11]);
	printf("  Token %zu: %s\n", (size_t)12, tokenNames[12]);
	printf("  Token %zu: %s\n", (size_t)13, tokenNames[13]);

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

		// Remover aspas de contorno se existirem
		char* input = line;
		char inputDisplay[512];
		strcpy(inputDisplay, line);

		if (line[0] == '\"' && line[strlen(line)-1] == '\"') {
			input = &line[1];
			line[strlen(line)-1] = '\0';
		}

		// Processar sequências de escape
		char processedInput[512];
		int processedLen = 0;
		for (int j = 0; input[j] && processedLen < 511; j++) {
			if (input[j] == '\\' && input[j+1]) {
				if (input[j+1] == 'b') {
					processedInput[processedLen++] = '\b';
					j++;
				} else if (input[j+1] == 'n') {
					processedInput[processedLen++] = '\n';
					j++;
				} else if (input[j+1] == 't') {
					processedInput[processedLen++] = '\t';
					j++;
				} else {
					processedInput[processedLen++] = input[j];
				}
			} else {
				processedInput[processedLen++] = input[j];
			}
		}
		processedInput[processedLen] = '\0';

		printf("Entrada: %s\n", inputDisplay);

		int pos = 0;
		while (processedInput[pos] != '\0') {
			// Ignorar espaços em branco entre tokens
			while (processedInput[pos] != '\0' && isspace(processedInput[pos])) {
				pos++;
			}
			if (processedInput[pos] == '\0') break;

			struct Token token = scanToken(processedInput, &pos);

			if (token.type >= 0) {
				printf("  -> %s ('", tokenNames[token.type]);
				for (int i = 0; token.value[i] != '\0'; i++) {
					if (token.value[i] >= 32 && token.value[i] < 127) {
						printf("%c", token.value[i]);
					} else {
						printf("[%d]", (int)(unsigned char)token.value[i]);
					}
				}
				printf("')\n");
			} else {
				printf("  -> Não reconhecido: '%c'\n", processedInput[pos]);
				pos++; // Avança um caractere para tentar continuar
			}
		}
		printf("\n");
	}

	fclose(file);
	printf("\n");
	return 0;
}
