#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Token {
	char type[64];       // Token name
	char value[256];     // Valor do token
};

// Função helper para comparar símbolo no input
int matchSymbol(const char* input, int pos, const char* symbol) {
	size_t len = strlen(symbol);
	if (strncmp(&input[pos], symbol, len) == 0) return len;
	return 0;
}

struct Token scanToken(const char* input, int* pos, const char* tokenNames[8]) {
	struct Token token;
	memset(token.type, 0, sizeof(token.type));
	memset(token.value, 0, sizeof(token.value));
	strcpy(token.type, "ERROR");

	int currentState = 0;
	int lastFinalState = -1;
	int lastFinalPos = *pos;
	int tokenStart = *pos;

	while (input[*pos] != '\0') {
		int nextState = -1;
		int symbolLen = 0;

		switch (currentState) {
			case 0: {
				if (input[*pos] == '0') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == '1') {
					nextState = 1;
					symbolLen = 1;
				}
 else if (input[*pos] == '2') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == '3') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == '4') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == '5') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == '6') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == '7') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == '8') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == '9') {
					nextState = 2;
					symbolLen = 1;
				}
 else if (input[*pos] == 'a') {
					nextState = 3;
					symbolLen = 1;
				}
 else if (input[*pos] == 'x') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (input[*pos] == 'y') {
					nextState = 4;
					symbolLen = 1;
				}
 else if (input[*pos] == 'z') {
					nextState = 5;
					symbolLen = 1;
				}
				break;
			}

			case 1: {
				if (input[*pos] == '1') {
					nextState = 1;
					symbolLen = 1;
				}
				break;
			}

			case 2: {
				break;
			}

			case 3: {
				if ((symbolLen = matchSymbol(input, *pos, "\b"))) {
					nextState = 6;
				}
 else if ((symbolLen = matchSymbol(input, *pos, "\t"))) {
					nextState = 7;
				}
 else if (input[*pos] == 'b') {
					nextState = 8;
					symbolLen = 1;
				}
				break;
			}

			case 4: {
				if (input[*pos] == 'y') {
					nextState = 4;
					symbolLen = 1;
				}
				break;
			}

			case 5: {
				break;
			}

			case 6: {
				if (input[*pos] == 'c') {
					nextState = 9;
					symbolLen = 1;
				}
				break;
			}

			case 7: {
				if ((symbolLen = matchSymbol(input, *pos, "\t"))) {
					nextState = 7;
				}
				break;
			}

			case 8: {
				if ((symbolLen = matchSymbol(input, *pos, "input"))) {
					nextState = 12;
				}
 else if (input[*pos] == 'c') {
					nextState = 10;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 11;
					symbolLen = 1;
				}
				break;
			}

			case 9: {
				break;
			}

			case 10: {
				if (input[*pos] == 'a') {
					nextState = 13;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 11;
					symbolLen = 1;
				}
				break;
			}

			case 11: {
				if (input[*pos] == 'a') {
					nextState = 14;
					symbolLen = 1;
				}
				break;
			}

			case 12: {
				if ((symbolLen = matchSymbol(input, *pos, "input"))) {
					nextState = 12;
				}
				break;
			}

			case 13: {
				if (input[*pos] == 'b') {
					nextState = 15;
					symbolLen = 1;
				}
				break;
			}

			case 14: {
				if (input[*pos] == 'b') {
					nextState = 16;
					symbolLen = 1;
				}
				break;
			}

			case 15: {
				if (input[*pos] == 'c') {
					nextState = 17;
					symbolLen = 1;
				}
				break;
			}

			case 16: {
				if (input[*pos] == 'c') {
					nextState = 18;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 11;
					symbolLen = 1;
				}
				break;
			}

			case 17: {
				if (input[*pos] == 'a') {
					nextState = 13;
					symbolLen = 1;
				}
				break;
			}

			case 18: {
				if (input[*pos] == 'd') {
					nextState = 11;
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
			token.value[*pos - tokenStart + i] = input[*pos + i];
		}
		*pos += symbolLen;

		// Verificar se é estado final (longest match)
		switch (currentState) {
			case 1:
				lastFinalState = 6;
				lastFinalPos = *pos;
				break;
			case 2:
				lastFinalState = 6;
				lastFinalPos = *pos;
				break;
			case 3:
				lastFinalState = 0;
				lastFinalPos = *pos;
				break;
			case 4:
				lastFinalState = 7;
				lastFinalPos = *pos;
				break;
			case 5:
				lastFinalState = 7;
				lastFinalPos = *pos;
				break;
			case 7:
				lastFinalState = 5;
				lastFinalPos = *pos;
				break;
			case 8:
				lastFinalState = 3;
				lastFinalPos = *pos;
				break;
			case 9:
				lastFinalState = 1;
				lastFinalPos = *pos;
				break;
			case 10:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			case 11:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 12:
				lastFinalState = 3;
				lastFinalPos = *pos;
				break;
			case 17:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			default:
				break;
		}
	}

	// Retornar ao último estado final (longest match)
	if (lastFinalState != -1 && lastFinalState >= 0 && lastFinalState < 8) {
		strcpy(token.type, tokenNames[lastFinalState]);
		*pos = lastFinalPos;
		token.value[lastFinalPos - tokenStart] = '\0';
	}

	return token;
}

int main() {
	// Initialize token names array
	const char* tokenNames[8] = {
		"TOKEN_A",
		"TOKEN_SEQUENCE",
		"TOKEN_KLEENE",
		"TOKEN_INPUT",
		"TOKEN_OPTIONAL",
		"TOKEN_TAB",
		"TOKEN_DIGIT",
		"TOKEN_LETTER"
	};

	FILE* file = fopen("tests/test_input.txt", "r");
	if (!file) {
		perror("Erro ao abrir tests/test_input.txt");
		return 1;
	}

	printf("Scanner Léxico Gerado\n");
	printf("═════════════════════════════════════\n\n");

	printf("Expressões Regulares:\n");
	printf("  %s\n", tokenNames[0]);
	printf("  %s\n", tokenNames[1]);
	printf("  %s\n", tokenNames[2]);
	printf("  %s\n", tokenNames[3]);
	printf("  %s\n", tokenNames[4]);
	printf("  %s\n", tokenNames[5]);
	printf("  %s\n", tokenNames[6]);
	printf("  %s\n", tokenNames[7]);
	printf("\n");

	printf("Testes:\n");
	printf("─────────────────────────────────────\n");

	char line[512];

	while (fgets(line, sizeof(line), file)) {
		// Remover quebra de linha
		size_t len = strlen(line);
		if (len > 0 && line[len-1] == '\n') {
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

		printf("Entrada: %s → ", inputDisplay);

		int pos = 0;
		struct Token token = scanToken(processedInput, &pos, tokenNames);

		if (strcmp(token.type, "ERROR") != 0) {
			printf("%s ('", token.type);
			for (int i = 0; token.value[i] != '\0'; i++) {
				if (token.value[i] >= 32 && token.value[i] < 127) {
					printf("%c", token.value[i]);
				} else {
					printf("[%d]", (int)(unsigned char)token.value[i]);
				}
			}
			printf("')\n");
		} else {
			printf("Não reconhecido\n");
		}
	}

	fclose(file);
	printf("\n\n");
	return 0;
}
