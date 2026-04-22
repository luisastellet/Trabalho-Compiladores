// Implementação da função main para o scanner

#include <stdio.h>
#include <string.h>

struct Token {
	int type;
	char value[256];
};

struct Token scanToken(const char* input, int* pos);

int main() {
	printf("Scanner Léxico Gerado\n");
	printf("═════════════════════════════════════\n\n");
	
	printf("Expressões Regulares:\n");
	printf("  Token 0: a b . c . *       (ab, abc, abcc, ...)\n");
	printf("  Token 1: a \\b c . .       (a, \\b, c)\n");
	printf("  Token 2: a b . input * .   (ab, input, input*)\n");
	printf("  Token 3: a b . c ? d . . + (ab(c|d) repetido)\n\n");
	
	printf("Testes:\n");
	printf("─────────────────────────────────────\n");
	
	// Testes
	const char* tests[] = {"\b", "a", "\ba", "ab", "abc", "abcc", "abd"};
	int numTests = 7;
	
	for (int i = 0; i < numTests; i++) {
		struct Token token;
		int pos = 0;
		const char* input = tests[i];
		
		printf("Entrada: \"%s\" ", input);
		token = scanToken(input, &pos);
		
		if (token.type >= 0) {
			printf("→ Token %d ('%s')\n", token.type, token.value);
		} else {
			printf("→ Não reconhecido\n");
		}
	}
	
	return 0;
}
