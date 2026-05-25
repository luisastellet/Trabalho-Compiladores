#!/bin/bash

echo "=== TESTES COMPLETOS: Lambda, Let, Cond ==="
echo ""

# Teste 1: Lambda simples
echo "--- Teste 1: Lambda simples ---"
echo "(lambda (x y) (+ x y))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

# Teste 2: Let simples
echo "--- Teste 2: Let simples ---"
echo "(let ((x 5) (y 10)) (+ x y))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

# Teste 3: Cond com 3 cláusulas
echo "--- Teste 3: Cond com 3 cláusulas ---"
echo "(cond ((> x 0) 1) ((< x 0) -1) (else 0))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

# Teste 4: Lambda aninhada em Let
echo "--- Teste 4: Lambda aninhada em Let ---"
echo "(let ((f (lambda (x) (* x 2)))) (f 5))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

# Teste 5: Cond aninhado
echo "--- Teste 5: Cond aninhado ---"
echo "(cond ((> x 10) \"grande\") ((cond ((> x 0) \"pequeno\") (else \"zero\"))) (else \"negativo\"))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

# Teste 6: Let aninhado
echo "--- Teste 6: Let aninhado ---"
echo "(let ((x 5)) (let ((y 10)) (+ x y)))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

# Teste 7: Begin (múltiplas expressões)
echo "--- Teste 7: Begin (múltiplas expressões) ---"
echo "(begin (define x 5) (define y 10) (+ x y))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

# Teste 8: Lambda com múltiplos parâmetros
echo "--- Teste 8: Lambda com múltiplos parâmetros ---"
echo "(lambda (a b c d) (+ a b c d))" | ./transpilador > /dev/null 2>&1 && cat saida.py || echo "ERRO"
echo ""

