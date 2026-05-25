#!/bin/bash

echo "=== TESTES DE VERIFICAÇÃO DE TIPOS ==="
echo ""

# Teste 1: Erro - String + Number
echo "❌ Teste 1: Erro - String + Number"
echo '(+ "hello" 5)' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 2: Erro - Number * String
echo "❌ Teste 2: Erro - Number * String"
echo '(* 3 "texto")' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 3: Erro - Comparação String > Number
echo "❌ Teste 3: Erro - Comparação String > Number"
echo '(> "abc" 10)' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 4: Sucesso - Operação válida
echo "✅ Teste 4: Sucesso - Operação válida"
echo '(+ 3 5)' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 5: Erro - Lambda com erro de tipo
echo "❌ Teste 5: Erro - Lambda com erro de tipo"
echo '(lambda (x) (+ x "string"))' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 6: Erro - Let com erro de tipo
echo "❌ Teste 6: Erro - Let com erro de tipo"
echo '(let ((x 5)) (> "teste" x))' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 7: Sucesso - Lambda com operação válida
echo "✅ Teste 7: Sucesso - Lambda com operação válida"
echo '(lambda (x) (+ x 10))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 8: Sucesso - Let com operação válida
echo "✅ Teste 8: Sucesso - Let com operação válida"
echo '(let ((x 5)) (+ x 10))' | ./transpilador > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "✅ PASSOU"; else echo "❌ FALHOU"; fi
echo ""

# Teste 9: Erro - Divisão com String
echo "❌ Teste 9: Erro - Divisão com String"
echo '(/ 10 "dois")' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

# Teste 10: Erro - Comparação <= com String
echo "❌ Teste 10: Erro - Comparação <= com String"
echo '(<= "x" 5)' | ./transpilador > /dev/null 2>&1
if [ $? -ne 0 ]; then echo "✅ PASSOU (erro detectado)"; else echo "❌ FALHOU"; fi
echo ""

echo "=== TESTES FINALIZADOS ==="
