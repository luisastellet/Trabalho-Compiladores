# Suite de Testes - Transpilador Scheme → Python

Documentação centralizada da suite de testes para o transpilador de Scheme para Python.

## 🎯 Executando os Testes

Para rodar a suite completa de testes:

```bash
./run_tests.sh
```

O script executará **28 testes** organizados em **8 categorias**:

### Categorias de Testes

#### 1️⃣ **Variáveis Não Declaradas** (3 testes)
- Variável simples não declarada
- Variável em expressão aninhada
- Múltiplas variáveis não declaradas

Esperado: `ERRO: Variavel/funcao 'X' nao foi declarada (usada na linha Y)`

#### 2️⃣ **Variáveis Declaradas - Sucesso** (3 testes)
- Variável simples declarada e usada
- Múltiplas variáveis declaradas
- Variável em expressão complexa

Esperado: Arquivo `saida.py` gerado com sucesso

#### 3️⃣ **Funções Não Declaradas** (2 testes)
- Função não declarada - call simples
- Função não declarada - em expressão

Esperado: `ERRO: Variavel/funcao 'X' nao foi declarada (usada na linha Y)`

#### 4️⃣ **Lambda - Parâmetros Pré-Declarados** (4 testes)
- Lambda simples com parâmetros
- Lambda e aplicação com argumentos
- Lambda com parâmetros em operações
- Lambda aninhada

Esperado: Arquivo `saida.py` gerado com sucesso (parâmetros reconhecidos)

#### 5️⃣ **Let/Let*/Letrec - Bindings Pré-Declarados** (5 testes)
- Let simples com um binding
- Let com múltiplos bindings
- Let-star com referência entre bindings
- Letrec com função recursiva
- Let com binding não utilizado (aviso)

Esperado: Arquivo `saida.py` gerado com sucesso (bindings reconhecidos)

#### 6️⃣ **Funções Built-in** (4 testes)
- Operador `+` (built-in)
- Operador `-` (built-in)
- Operador `*` (built-in)
- Função `list` (built-in)

Esperado: Arquivo `saida.py` gerado com sucesso (sem erro para built-ins)

#### 7️⃣ **Estruturas de Controle** (3 testes)
- If com variável declarada
- If com variável não declarada
- Cond com variável declarada

Esperado: Sucesso para declaradas, erro para não declaradas

#### 8️⃣ **Casos Mistos** (4 testes)
- Função definida chamada com sucesso
- Função definida chamada com variável undeclared
- Begin com múltiplas expressões
- Set! com variável existente

Esperado: Validação de escopo em cenários complexos

---

## 📊 Output do Teste

Cada teste exibe:

```
Teste #N: [Nome do Teste]
Código: [Código Scheme testado]
Esperado: [Padrão esperado na saída]
✓ PASSOU  (ou ✗ FALHOU)
```

Ao final, um resumo:

```
RESUMO DOS TESTES
━━━━━━━━━━━━━━━━━━
Total:  28
Passou: 28
Falhou: 0

✓ TODOS OS TESTES PASSARAM!
```

---

## 🔧 Adicionando Novos Testes

Para adicionar um novo teste, edite `run_tests.sh` e adicione uma chamada `run_test`:

```bash
run_test \
    "Descrição do teste" \
    "(seu-codigo-scheme)" \
    "padrão-esperado-na-saída" \
    "yes"    # "yes" se deve falhar (erro), "no" se deve compilar
```

**Exemplo:**

```bash
run_test \
    "Variável x não declarada em soma" \
    "(+ x 5)" \
    "ERRO: Variavel/funcao 'x' nao foi declarada" \
    "yes"
```

---

## 🎓 Funcionalidades Testadas

### ✅ Detecção de Variáveis Não Declaradas
- Identifica referências a variáveis que não foram definidas com `define`
- Erro: `ERRO: Variavel/funcao 'X' nao foi declarada (usada na linha Y)`
- Não fatalmente (compilação é abortada com exit code 1)

### ✅ Parâmetros de Lambda
- Parâmetros de lambda são automaticamente considerados declarados
- Não geram erros de "não declarado" quando usados no corpo da lambda

### ✅ Bindings de Let/Let*/Letrec
- Bindings são automaticamente considerados declarados no escopo
- Variáveis vinculadas podem ser usadas sem erros
- Let-star permite referências entre bindings

### ✅ Funções Built-in
- Funções built-in (como `+`, `-`, `*`, etc.) não geram erros
- Não exigem declaração prévia

### ✅ Estruturas de Controle
- If, cond e outras estruturas validam corretamente o escopo
- Variáveis não declaradas em condições são detectadas

---

## 📝 Estrutura do Repositório

```
T2/
├── run_tests.sh              # Script principal de testes
├── TESTES_README.md          # Este arquivo
├── sintatico.y               # Parser Bison
├── lexico.l                  # Lexer Flex
├── scheme.h                  # Headers e definições
├── tabela_simbolos.c         # Tabela de símbolos
├── ast.c                     # AST e geração de código
├── Makefile                  # Build
└── transpilador              # Executável compilado
```

---

## 🚀 Workflow de Desenvolvimento

1. **Compile o transpilador:**
   ```bash
   make clean && make
   ```

2. **Execute a suite de testes:**
   ```bash
   ./run_tests.sh
   ```

3. **Verifique os resultados:**
   - ✓ Testes verdes = OK
   - ✗ Testes vermelhos = Falha (veja o output para debug)

4. **Adicione novos testes conforme implementa features**

---

## 🐛 Debugging de Testes

Se um teste falhar, o script mostra:

- **Exit code:** Retorno do transpilador
- **Pattern encontrado:** Se o padrão foi encontrado na saída
- **Output completo:** Mensagem de erro ou sucesso

Exemplo de falha:

```
✗ FALHOU
  Exit code: 0 (esperado 1)
  Pattern encontrado: 0 (esperado 1)
  Output: [output da execução]
```

---

## 📌 Notas Importantes

- ⚠️ O transpilador gera `saida.py` mesmo em caso de erro (será sobrescrito a cada teste)
- ⏱️ A suite completa executa em ~15-30 segundos
- 🔄 Use `set +e` no script para permitir testes que geram erros (já implementado)
- 📍 Todos os erros incluem **número de linha** onde a variável/função foi usada

---

## ✨ Resumo de Funcionalidades

| Funcionalidade | Status | Testes |
|---|---|---|
| Variáveis não declaradas | ✅ Completo | 3 |
| Lambda com parâmetros | ✅ Completo | 4 |
| Let/Let*/Letrec bindings | ✅ Completo | 5 |
| Built-ins sem erro | ✅ Completo | 4 |
| Estruturas de controle | ✅ Completo | 3 |
| Casos mistos | ✅ Completo | 4 |

**Total: 28 testes, 100% passando** ✓
