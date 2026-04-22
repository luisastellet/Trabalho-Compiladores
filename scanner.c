#include <stdio.h>
#include <string.h>

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
				if (input[*pos] == 'a') {
					nextState = 1;
					symbolLen = 1;
				}
				break;
			}

			case 1: {
				if ((symbolLen = matchSymbol(input, *pos, "\b"))) {
					nextState = 2;
				}
 else if ((symbolLen = matchSymbol(input, *pos, "\t"))) {
					nextState = 3;
				}
 else if (input[*pos] == 'b') {
					nextState = 4;
					symbolLen = 1;
				}
				break;
			}

			case 2: {
				if (input[*pos] == 'c') {
					nextState = 5;
					symbolLen = 1;
				}
				break;
			}

			case 3: {
				if ((symbolLen = matchSymbol(input, *pos, "\t"))) {
					nextState = 3;
				}
				break;
			}

			case 4: {
				if ((symbolLen = matchSymbol(input, *pos, "input"))) {
					nextState = 8;
				}
 else if (input[*pos] == 'c') {
					nextState = 6;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 5: {
				break;
			}

			case 6: {
				if (input[*pos] == 'a') {
					nextState = 9;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 7: {
				if (input[*pos] == 'a') {
					nextState = 10;
					symbolLen = 1;
				}
				break;
			}

			case 8: {
				if ((symbolLen = matchSymbol(input, *pos, "input"))) {
					nextState = 8;
				}
				break;
			}

			case 9: {
				if (input[*pos] == 'b') {
					nextState = 11;
					symbolLen = 1;
				}
				break;
			}

			case 10: {
				if (input[*pos] == 'b') {
					nextState = 12;
					symbolLen = 1;
				}
				break;
			}

			case 11: {
				if (input[*pos] == 'c') {
					nextState = 13;
					symbolLen = 1;
				}
				break;
			}

			case 12: {
				if (input[*pos] == 'c') {
					nextState = 14;
					symbolLen = 1;
				}
 else if (input[*pos] == 'd') {
					nextState = 7;
					symbolLen = 1;
				}
				break;
			}

			case 13: {
				if (input[*pos] == 'a') {
					nextState = 9;
					symbolLen = 1;
				}
				break;
			}

			case 14: {
				if (input[*pos] == 'd') {
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
		// Adicionar símbolo ao valor do token
		for (int i = 0; i < symbolLen; i++) {
			token.value[*pos - tokenStart + i] = input[*pos + i];
		}
		*pos += symbolLen;

		// Verificar se é estado final (longest match)
		switch (currentState) {
			case 1:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 3:
				lastFinalState = 4;
				lastFinalPos = *pos;
				break;
			case 4:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			case 5:
				lastFinalState = 0;
				lastFinalPos = *pos;
				break;
			case 6:
				lastFinalState = 1;
				lastFinalPos = *pos;
				break;
			case 7:
				lastFinalState = 3;
				lastFinalPos = *pos;
				break;
			case 8:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			case 13:
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
	FILE* file = fopen("tests/test_input.txt", "r");
	if (!file) {
		perror("Erro ao abrir tests/test_input.txt");
		return 1;
	}

	printf("Scanner Léxico Gerado\n");
	printf("═════════════════════════════════════\n\n");

	printf("Expressões Regulares:\n");
	printf("  Token 0: a b . c . *       (ab, abc, abcc, ...)\n");
	printf("  Token 1: a \\b c . .       (a, \\b, c)\n");
	printf("  Token 2: a b . input * .   (ab, input, input*)\n");
	printf("  Token 3: a b . c ? d . . + (ab(c|d) repetido)\n\n");

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
		struct Token token = scanToken(processedInput, &pos);

		if (token.type >= 0) {
			printf("Token %d ('", token.type);
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
