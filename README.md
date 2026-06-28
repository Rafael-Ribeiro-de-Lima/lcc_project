# INE5622 - Analisador Léxico e Sintático LL(1) para LCC-2026-1

Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995

Este projeto implementa, em C++, um analisador léxico e um analisador sintático preditivo tabular para a linguagem LCC-2026-1 do Exercício-Programa de INE5622 - Introdução a Compiladores.

## 1. Ambiente e versão

- Linguagem: C++
- Compilador previsto pelo enunciado: `g++ 12.2.0`
- Padrão usado: `C++17`
- Ambiente: Linux/Unix
- Bibliotecas externas: nenhuma

O projeto usa apenas biblioteca padrão de C++.

## 2. Estrutura dos arquivos

```text
.
├── Makefile
├── README.md
├── main.cpp
├── token.hpp
├── token.cpp
├── errors.hpp
├── errors.cpp
├── symbol_table.hpp
├── symbol_table.cpp
├── lexer.hpp
├── lexer.cpp
├── grammar.hpp
├── grammar.cpp
├── parser_ll1.hpp
├── parser_ll1.cpp
├── programa1.lcc
├── programa2.lcc
└── programa3.lcc
```

Papel de cada arquivo:

- `main.cpp`: ponto de entrada; abre o arquivo `.lcc`, executa lexer, imprime tokens/tabela de símbolos e executa parser.
- `token.hpp` / `token.cpp`: definição dos tokens, palavras reservadas e conversão para terminais da gramática.
- `errors.hpp` / `errors.cpp`: classes de erro léxico e sintático.
- `symbol_table.hpp` / `symbol_table.cpp`: tabela de símbolos de identificadores e ocorrências.
- `lexer.hpp` / `lexer.cpp`: analisador léxico caractere por caractere.
- `grammar.hpp` / `grammar.cpp`: gramática, FIRST, FOLLOW e tabela de reconhecimento sintático.
- `parser_ll1.hpp` / `parser_ll1.cpp`: analisador sintático tabular.
- `programa1.lcc`, `programa2.lcc`, `programa3.lcc`: programas validos com mais de 100 linhas cada.

## 3. Como compilar

```bash
make
```

Ou o projeto também pode ser compilado diretamente com g++:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o lcc_analyzer main.cpp token.cpp errors.cpp symbol_table.cpp lexer.cpp grammar.cpp parser_ll1.cpp
```

Esses comandos geram o executavel:

```text
lcc_analyzer
```

O `Makefile` usa:

```make
CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
```

## 4. Como executar sobre um arquivo específico

```bash
make run FILE=programa1.lcc
```

Também é possível executar diretamente:

```bash
./lcc_analyzer programa1.lcc
```

## 5. Como testar os três programas válidos

```bash
make test
```

Saída esperada:

```text
OK - programa1.lcc
OK - programa2.lcc
OK - programa3.lcc
Todos os programas válidos foram aceitos.
```

## 6. Como limpar arquivos temporários

```bash
make clean
```

## 7. Analisador léxico

O analisador léxico está em `lexer.cpp` e lê o arquivo caractere por caractere.

Ele reconhece:

### Palavras reservadas

```text
def int float string print read return if else for break new null
```

### Tokens não triviais

```text
ident
int_constant
float_constant
string_constant
```

### Operadores

```text
+ - * / % < > <= >= == != =
```

### Delimitadores

```text
; , ( ) { } [ ]
```

### Comentários

Como extensão apenas léxical, o lexer ignora:

```text
// comentário de linha
/* comentário de bloco */
```

A inclusão de comentários não altera a gramática sintática, pois comentários são tratados como espaço em branco.

## 8. Tabela de símbolos

A tabela de símbolos é implementada em `symbol_table.cpp`.

Ela possui:

- uma entrada por identificador distinto;
- lista de ocorrências de cada identificador;
- linha e coluna inicial de cada ocorrência.

Exemplo de impressão:

```text
TABELA DE SÍMBOLOS
Identificador | Ocorrências (linha,coluna)
------------------------------------------
A             | (3,5), (4,1)
principal     | (1,5)
```

## 9. Saída do analisador léxico

Se não houver erro léxico, o programa imprime:

1. lista de tokens na ordem em que aparecem no arquivo;
2. tabela de símbolos.

Exemplo:

```text
LISTA DE TOKENS
[def, ident(principal), (, ), {, int, ident(A), ;, ident(A), =, int_constant(10), ;, return, ;, }]
```

Se houver erro léxico, imprime mensagem simples com linha e coluna:

```text
ERRO LÉXICO: linha 5, coluna 7: caractere inválido '@'
```

## 10. Analisador sintático tabular

O analisador sintático está em `parser_ll1.cpp`.

Ele é tabular:

1. a gramática final é definida em `grammar.cpp`;
2. FIRST e FOLLOW são construídos automaticamente;
3. a tabela de reconhecimento sintático é construída uma única vez quando o objeto `Grammar` é criado;
4. o parser usa pilha e consulta a tabela `M[A,a]`;
5. não há parser recursivo descendente manual.

## 11. Saída do analisador sintático

Se a entrada pertence a linguagem:

```text
ANÁLISE SINTÁTICA: sucesso. A entrada pertence a linguagem LCC-2026-1.
```

Se houver erro sintático por entrada vazia na tabela:

```text
ERRO SINTÁTICO: entrada vazia na tabela de reconhecimento sintático M[TERM_TAIL, print]
forma sentencial alpha: TERM_TAIL NUMEXPRESSION_TAIL REL_OPT ; STMT_LIST_TAIL } FUNC_LIST_TAIL
não-terminal mais à esquerda de alpha: TERM_TAIL
token atual da entrada: print (lexema 'print', linha 5, coluna 1)
```

Se ocorrer desencontro direto de terminal, a mensagem também informa a forma sentencial, o símbolo relevante e o token atual.

## 12. Gramática final usada

A gramática abaixo é a conversão da BNF original para forma convencional. Opcionais, repetições e alternativas foram expandidas por novos não-terminais; isso não restringe nem amplia os comandos da BNF original.

```text
PROGRAM -> PROGRAM_OPT
PROGRAM_OPT -> FUNC_LIST
PROGRAM_OPT -> STATEMENT
PROGRAM_OPT -> ε

FUNC_LIST -> FUNCDEF FUNC_LIST_TAIL
FUNC_LIST_TAIL -> FUNCDEF FUNC_LIST_TAIL
FUNC_LIST_TAIL -> ε

FUNCDEF -> def ident ( PARAMS_OPT ) { STMT_LIST }
PARAMS_OPT -> PARAM_LIST
PARAMS_OPT -> ε
PARAM_LIST -> TYPE ident PARAM_TAIL
PARAM_TAIL -> , PARAM_LIST
PARAM_TAIL -> ε

TYPE -> int
TYPE -> float
TYPE -> string

STMT_LIST -> STATEMENT STMT_LIST_TAIL
STMT_LIST_TAIL -> STATEMENT STMT_LIST_TAIL
STMT_LIST_TAIL -> ε

STATEMENT -> VARDECL ;
STATEMENT -> ATTRIBSTAT ;
STATEMENT -> PRINTSTAT ;
STATEMENT -> READSTAT ;
STATEMENT -> RETURNSTAT ;
STATEMENT -> IFSTAT
STATEMENT -> FORSTAT
STATEMENT -> { STMT_LIST }
STATEMENT -> break ;
STATEMENT -> ;

VARDECL -> TYPE ident ARRAY_DECL_LIST
ARRAY_DECL_LIST -> [ int_constant ] ARRAY_DECL_LIST
ARRAY_DECL_LIST -> ε

ATTRIBSTAT -> LVALUE = ATTRIB_RHS
ATTRIB_RHS -> ALLOCEXPRESSION
ATTRIB_RHS -> ident ATTRIB_RHS_IDENT_TAIL
ATTRIB_RHS -> NUMEXPRESSION_NON_IDENT_START REL_OPT
ATTRIB_RHS_IDENT_TAIL -> ( PARAMLIST_CALL_OPT )
ATTRIB_RHS_IDENT_TAIL -> INDEX_LIST TERM_TAIL NUMEXPRESSION_TAIL REL_OPT

NUMEXPRESSION_NON_IDENT_START -> TERM_NON_IDENT_START NUMEXPRESSION_TAIL
TERM_NON_IDENT_START -> UNARY_EXPR_NON_IDENT_START TERM_TAIL
UNARY_EXPR_NON_IDENT_START -> + FACTOR
UNARY_EXPR_NON_IDENT_START -> - FACTOR
UNARY_EXPR_NON_IDENT_START -> FACTOR_NON_IDENT
FACTOR_NON_IDENT -> int_constant
FACTOR_NON_IDENT -> float_constant
FACTOR_NON_IDENT -> string_constant
FACTOR_NON_IDENT -> null
FACTOR_NON_IDENT -> ( NUMEXPRESSION )

PARAMLIST_CALL_OPT -> PARAMLIST_CALL
PARAMLIST_CALL_OPT -> ε
PARAMLIST_CALL -> ident PARAMLIST_CALL_TAIL
PARAMLIST_CALL_TAIL -> , PARAMLIST_CALL
PARAMLIST_CALL_TAIL -> ε

PRINTSTAT -> print EXPRESSION
READSTAT -> read LVALUE
RETURNSTAT -> return

IFSTAT -> if ( EXPRESSION ) STATEMENT ELSE_OPT
ELSE_OPT -> else STATEMENT
ELSE_OPT -> ε

FORSTAT -> for ( ATTRIBSTAT ; EXPRESSION ; ATTRIBSTAT ) STATEMENT

ALLOCEXPRESSION -> new TYPE ALLOC_DIMS
ALLOC_DIMS -> [ NUMEXPRESSION ] ALLOC_DIMS_TAIL
ALLOC_DIMS_TAIL -> [ NUMEXPRESSION ] ALLOC_DIMS_TAIL
ALLOC_DIMS_TAIL -> ε

EXPRESSION -> NUMEXPRESSION REL_OPT
REL_OPT -> REL_OP NUMEXPRESSION
REL_OPT -> ε
REL_OP -> <
REL_OP -> >
REL_OP -> <=
REL_OP -> >=
REL_OP -> ==
REL_OP -> !=

NUMEXPRESSION -> TERM NUMEXPRESSION_TAIL
NUMEXPRESSION_TAIL -> + TERM NUMEXPRESSION_TAIL
NUMEXPRESSION_TAIL -> - TERM NUMEXPRESSION_TAIL
NUMEXPRESSION_TAIL -> ε

TERM -> UNARY_EXPR TERM_TAIL
TERM_TAIL -> * UNARY_EXPR TERM_TAIL
TERM_TAIL -> / UNARY_EXPR TERM_TAIL
TERM_TAIL -> % UNARY_EXPR TERM_TAIL
TERM_TAIL -> ε

UNARY_EXPR -> + FACTOR
UNARY_EXPR -> - FACTOR
UNARY_EXPR -> FACTOR

FACTOR -> int_constant
FACTOR -> float_constant
FACTOR -> string_constant
FACTOR -> null
FACTOR -> LVALUE
FACTOR -> ( NUMEXPRESSION )

LVALUE -> ident INDEX_LIST
INDEX_LIST -> [ NUMEXPRESSION ] INDEX_LIST
INDEX_LIST -> ε
```

### Observacao sobre o `else` opcional

Na producao de comandos condicionais, a parte `else` e opcional:

```text
IFSTAT -> if ( EXPRESSION ) STATEMENT ELSE_OPT
ELSE_OPT -> else STATEMENT
ELSE_OPT -> ε
```

Quando o token atual da entrada e else, a tabela de reconhecimento sintatico seleciona a producao ELSE_OPT -> else STATEMENT. Assim, o else fica associado ao if mais proximo, que e a convencao usual para comandos condicionais encadeados.