#!/bin/bash

echo "=== TESTES DE INTEGRAÇÃO: Type Checking + Lambda/Let/Cond ==="
echo ""

# Teste 1: Lambda válida com operação aritmética
echo "✅ Teste 1: Lambda válida"
echo '(lambda (x y) (+ x y))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 2: Lambda INVÁLIDA (tipo errado no corpo)
echo "❌ Teste 2: Lambda com erro de tipo no corpo"
echo '(lambda (x) (+ x "erro"))' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 3: Let válido com operação aritmética
echo "✅ Teste 3: Let válido"
echo '(let ((x 5) (y 10)) (+ x y))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 4: Let INVÁLIDO (tipo errado na operação)
echo "❌ Teste 4: Let com erro de tipo"
echo '(let ((x 5)) (* x "invalido"))' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 5: Cond válido com comparações numéricas
echo "✅ Teste 5: Cond válido"
echo '(cond ((> x 0) 1) ((< x 0) -1) (else 0))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 6: Cond INVÁLIDO (comparação com string)
echo "❌ Teste 6: Cond com comparação inválida"
echo '(cond ((> "texto" 5) 1) (else 0))' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 7: Lambda aninhada em Let (válido)
echo "✅ Teste 7: Lambda aninhada em Let"
echo '(let ((f (lambda (x) (* x 2)))) (f 5))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 8: Let aninhado com Cond (válido)
echo "✅ Teste 8: Let aninhado com Cond"
echo '(let ((x 10)) (cond ((> x 5) "grande") (else "pequeno")))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 9: Begin com múltiplas operações válidas
echo "✅ Teste 9: Begin válido"
echo '(begin (define x 5) (+ x 10))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 10: Begin com operação inválida
echo "❌ Teste 10: Begin com erro"
echo '(begin (define x 5) (+ x "erro"))' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

echo "=== TESTES FINALIZADOS ==="
