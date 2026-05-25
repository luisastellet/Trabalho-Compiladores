#!/bin/bash

echo "=========================================="
echo "DEMONSTRACAO: Type Checking com Linhas"
echo "=========================================="
echo ""

# Teste 1: Erro na linha 1
echo "1. Erro na Linha 1:"
echo "(+ \"hello\" 5)" | ./transpilador 2>&1 | grep "ERRO DE TIPO"
echo ""

# Teste 2: Erro na linha 2
echo "2. Erro na Linha 2:"
printf "(define x 5)\n(* 3 \"texto\")\n(+ 10 20)" | ./transpilador 2>&1 | grep "ERRO DE TIPO"
echo ""

# Teste 3: Erro na linha 3
echo "3. Erro na Linha 3:"
printf "(define x 5)\n(define y 10)\n(> \"abc\" x)" | ./transpilador 2>&1 | grep "ERRO DE TIPO"
echo ""

# Teste 4: Lambda com erro aninhado
echo "4. Lambda com erro na linha 1:"
echo '(lambda (x) (+ x "string"))' | ./transpilador 2>&1 | grep "ERRO DE TIPO"
echo ""

# Teste 5: Operação válida
echo "5. Operacao valida (sem erros):"
echo "(+ 3 5)" | ./transpilador 2>&1 | grep "ERRO"
if [ $? -ne 0 ]; then echo "[Nenhum erro detectado - correto!]"; fi
echo ""

echo "=========================================="
echo "Notem:"
echo "- Emojis removidos das mensagens"
echo "- Numero da linha reportado"
echo "- Mensagens claras ao programador"
echo "=========================================="
