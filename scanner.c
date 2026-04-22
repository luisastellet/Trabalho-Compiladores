#include <stdio.h>
#include <string.h>

struct Token {
	int type;        // ID do token (-1 = erro)
	char value[256]; // Valor do token
};

struct Token scanToken(const char* input, int* pos) {
	struct Token token;
	token.type = -1;
	memset(token.value, 0, sizeof(token.value));

	int currentState = 0;
	int lastFinalState = -1;
	int lastFinalPos = *pos;
	int tokenStart = *pos;

	while (input[*pos] != '\0') {
		char c = input[*pos];
		int nextState = -1;

		switch (currentState) {
			case 0:
				switch (c) {
					case 'a':
						nextState = 1;
						break;
				}
				break;

			case 1:
				switch (c) {
					case '\b':
						nextState = 2;
						break;
					case 'b':
						nextState = 3;
						break;
				}
				break;

			case 2:
				switch (c) {
					case 'c':
						nextState = 4;
						break;
				}
				break;

			case 3:
				switch (c) {
					case 'c':
						nextState = 5;
						break;
					case 'd':
						nextState = 6;
						break;
				}
				break;

			case 4:
				switch (c) {
					default:
						nextState = -1;
				}
				break;

			case 5:
				switch (c) {
					case 'a':
						nextState = 8;
						break;
					case 'd':
						nextState = 6;
						break;
				}
				break;

			case 6:
				switch (c) {
					case 'a':
						nextState = 9;
						break;
				}
				break;

			case 7:
				switch (c) {
					default:
						nextState = -1;
				}
				break;

			case 8:
				switch (c) {
					case 'b':
						nextState = 10;
						break;
				}
				break;

			case 9:
				switch (c) {
					case 'b':
						nextState = 11;
						break;
				}
				break;

			case 10:
				switch (c) {
					case 'c':
						nextState = 12;
						break;
				}
				break;

			case 11:
				switch (c) {
					case 'c':
						nextState = 13;
						break;
					case 'd':
						nextState = 6;
						break;
				}
				break;

			case 12:
				switch (c) {
					case 'a':
						nextState = 8;
						break;
				}
				break;

			case 13:
				switch (c) {
					case 'd':
						nextState = 6;
						break;
				}
				break;

			default:
				nextState = -1;
				break;
		}

		if (nextState == -1) break;

		currentState = nextState;
		token.value[*pos - tokenStart] = c;
		(*pos)++;

		// Verificar se é estado final (longest match)
		switch (currentState) {
			case 3:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			case 4:
				lastFinalState = 0;
				lastFinalPos = *pos;
				break;
			case 5:
				lastFinalState = 1;
				lastFinalPos = *pos;
				break;
			case 6:
				lastFinalState = 3;
				lastFinalPos = *pos;
				break;
			case 7:
				lastFinalState = 2;
				lastFinalPos = *pos;
				break;
			case 12:
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

		printf("Entrada: %s → ", inputDisplay);

		int pos = 0;
		struct Token token = scanToken(input, &pos);

		if (token.type >= 0) {
			printf("Token %d ('%s')\n", token.type, token.value);
		} else {
			printf("Não reconhecido\n");
		}
	}

	fclose(file);
	printf("\n\n");
	return 0;
}
