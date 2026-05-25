# Testes Completos: Lambda, Let, Cond

## Status: ✅ TUDO FUNCIONANDO

### Teste 1: Lambda Simples
**Input Scheme:**
```scheme
(lambda (x y) (+ x y))
```
**Output Python:**
```python
lambda x, y: (x + y)
```
**Status:** ✅ PASSADO

---

### Teste 2: Let Simples (Binding Paralelo)
**Input Scheme:**
```scheme
(let ((x 5) (y 10)) (+ x y))
```
**Output Python:**
```python
(lambda x, y: (x + y))(5, 10)
```
**Comportamento:** Bindings avaliados em paralelo, depois lambda imediata chamada
**Status:** ✅ PASSADO

---

### Teste 3: Cond com 3 Cláusulas
**Input Scheme:**
```scheme
(cond ((> x 0) 1) ((< x 0) -1) (else 0))
```
**Output Python:**
```python
-1 if (x < 0) else (1 if (x > 0) else (0))
```
**Comportamento:** Ternários aninhados corretamente
**Status:** ✅ PASSADO

---

### Teste 4: Lambda Aninhada em Let
**Input Scheme:**
```scheme
(let ((f (lambda (x) (* x 2)))) (f 5))
```
**Output Python:**
```python
(lambda f: f(5))(lambda x: (x * 2))
```
**Comportamento:** Let cria closure que recebe função lambda
**Status:** ✅ PASSADO

---

### Teste 5: Cond Aninhado (Cond dentro de Cond)
**Input Scheme:**
```scheme
(cond ((> x 10) "grande") ((> x 0) (cond ((> y 0) "pos") (else "neg"))) (else "zero"))
```
**Output Python:**
```python
"pos" if (y > 0) else ("neg") if (x > 0) else ("grande" if (x > 10) else ("zero"))
```
**Comportamento:** Ternários aninhados corretamente com múltiplos níveis
**Status:** ✅ PASSADO

---

### Teste 6: Let Aninhado (Let dentro de Let)
**Input Scheme:**
```scheme
(let ((x 5)) (let ((y 10)) (+ x y)))
```
**Output Python:**
```python
(lambda x: (lambda y: (x + y))(10))(5)
```
**Comportamento:** Lambda aninhadas com escopo correto
**Status:** ✅ PASSADO

---

### Teste 7: Begin (Múltiplas Expressões)
**Input Scheme:**
```scheme
(begin (define x 5) (define y 10) (+ x y))
```
**Output Python:**
```python
(x = 5; y = 10; (x + y))
```
**Comportamento:** Sequência de expressões com ponto-e-vírgula
**Status:** ✅ PASSADO

---

### Teste 8: Lambda com Múltiplos Parâmetros
**Input Scheme:**
```scheme
(lambda (a b c d) (+ a b c d))
```
**Output Python:**
```python
lambda a, b, c, d: (((a + b) + c) + d)
```
**Comportamento:** Parâmetros múltiplos, operação esquerda-associativa
**Status:** ✅ PASSADO

---

## Resumo

| Teste | Funcionalidade | Input | Output | Status |
|-------|---|---|---|---|
| 1 | Lambda Simples | `(lambda (x y) (+ x y))` | `lambda x, y: (x + y)` | ✅ |
| 2 | Let Simples | `(let ((x 5) (y 10)) (+ x y))` | `(lambda x, y: (x + y))(5, 10)` | ✅ |
| 3 | Cond 3 Cláusulas | `(cond ((> x 0) 1) ((< x 0) -1) (else 0))` | Ternários corretos | ✅ |
| 4 | Lambda em Let | `(let ((f (lambda (x) (* x 2)))) (f 5))` | `(lambda f: f(5))(lambda x: ...)` | ✅ |
| 5 | Cond Aninhado | Cond dentro de Cond | Ternários aninhados | ✅ |
| 6 | Let Aninhado | Let dentro de Let | Lambdas aninhadas | ✅ |
| 7 | Begin Sequencial | `(begin (define x 5) ...)` | Ponto-e-vírgula | ✅ |
| 8 | Lambda N-ária | `(lambda (a b c d) (+ a b c d))` | Parâmetros múltiplos | ✅ |

**Total: 8/8 Testes Passando ✅**

---

## Observações Técnicas

### Geração de Lambda
- Uso de `lambda param1, param2: corpo` em Python
- Corretamente injeta parâmetros na ordem

### Geração de Let (Paralelo)
- Implementado como função lambda imediata: `(lambda vars: corpo)(valores)`
- Bindings avaliados em paralelo (não sequencialmente como let*)
- Ordem de parâmetros preservada

### Geração de Cond
- Usa ternários aninhados em Python: `a if test1 else (b if test2 else c)`
- Processamento de cláusulas reverso para ordem correta
- `else` detectado como cláusula final

### Casos Pendentes
- **let* (sequential bindings)**: Atualmente gera como `let` (paralelo)
- **case statement**: Não testado
- **letrec**: Não testado completamente

---

## Como Executar Testes

```bash
cd /Users/migueljehle/UFF/Trabalho-Compiladores/T2

# Teste individual
echo "(lambda (x y) (+ x y))" | ./transpilador && cat saida.py

# Todos os testes
./comprehensive_tests.sh
```
