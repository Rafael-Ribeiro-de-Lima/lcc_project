# INE5622 - Analisador Lexico e Sintatico LL(1) para LCC-2026-1

Integrantes do grupo: preencher antes da entrega.

Este projeto implementa, em C++, um analisador lexico e um analisador sintatico preditivo tabular para a linguagem LCC-2026-1 do Exercício-Programa de INE5622 - Introducao a Compiladores.

## 1. Ambiente e versao

- Linguagem: C++
- Compilador previsto pelo enunciado: `g++ 12.2.0`
- Padrao usado: `C++17`
- Ambiente: Linux/Unix
- Bibliotecas externas: nenhuma

O projeto usa apenas biblioteca padrao de C++.

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

- `main.cpp`: ponto de entrada; abre o arquivo `.lcc`, executa lexer, imprime tokens/tabela de simbolos e executa parser.
- `token.hpp` / `token.cpp`: definicao dos tokens, palavras reservadas e conversao para terminais da gramatica.
- `errors.hpp` / `errors.cpp`: classes de erro lexico e sintatico.
- `symbol_table.hpp` / `symbol_table.cpp`: tabela de simbolos de identificadores e ocorrencias.
- `lexer.hpp` / `lexer.cpp`: analisador lexico caractere por caractere.
- `grammar.hpp` / `grammar.cpp`: gramatica, FIRST, FOLLOW e tabela de reconhecimento sintatico.
- `parser_ll1.hpp` / `parser_ll1.cpp`: analisador sintatico tabular.
- `programa1.lcc`, `programa2.lcc`, `programa3.lcc`: programas validos com mais de 100 linhas cada.

## 3. Como compilar

```bash
make
```

Ou o projeto tambem pode ser compilado diretamente com g++:

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

## 4. Como executar sobre um arquivo especifico

```bash
make run FILE=programa1.lcc
```

Tambem e possivel executar diretamente:

```bash
./lcc_analyzer programa1.lcc
```

## 5. Como testar os tres programas validos

```bash
make test
```

Saida esperada:

```text
OK - programa1.lcc
OK - programa2.lcc
OK - programa3.lcc
Todos os programas validos foram aceitos.
```

## 6. Como limpar arquivos temporarios

```bash
make clean
```

## 7. Analisador lexico

O analisador lexico esta em `lexer.cpp` e le o arquivo caractere por caractere.

Ele reconhece:

### Palavras reservadas

```text
def int float string print read return if else for break new null
```

### Tokens nao triviais

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

### Comentarios

Como extensao apenas lexical, o lexer ignora:

```text
// comentario de linha
/* comentario de bloco */
```

A inclusao de comentarios nao altera a gramatica sintatica, pois comentarios sao tratados como espaco em branco.

## 8. Tabela de simbolos

A tabela de simbolos e implementada em `symbol_table.cpp`.

Ela possui:

- uma entrada por identificador distinto;
- lista de ocorrencias de cada identificador;
- linha e coluna inicial de cada ocorrencia.

Exemplo de impressao:

```text
TABELA DE SIMBOLOS
Identificador | Ocorrencias (linha,coluna)
------------------------------------------
A             | (3,5), (4,1)
principal     | (1,5)
```

## 9. Saida do analisador lexico

Se nao houver erro lexico, o programa imprime:

1. lista de tokens na ordem em que aparecem no arquivo;
2. tabela de simbolos.

Exemplo:

```text
LISTA DE TOKENS
[def, ident(principal), (, ), {, int, ident(A), ;, ident(A), =, int_constant(10), ;, return, ;, }]
```

Se houver erro lexico, imprime mensagem simples com linha e coluna:

```text
ERRO LEXICO: linha 5, coluna 7: caractere invalido '@'
```

## 10. Analisador sintatico tabular

O analisador sintatico esta em `parser_ll1.cpp`.

Ele e tabular:

1. a gramatica final e definida em `grammar.cpp`;
2. FIRST e FOLLOW sao construidos automaticamente;
3. a tabela de reconhecimento sintatico e construida uma unica vez quando o objeto `Grammar` e criado;
4. o parser usa pilha e consulta a tabela `M[A,a]`;
5. nao ha parser recursivo descendente manual.

## 11. Saida do analisador sintatico

Se a entrada pertence a linguagem:

```text
ANALISE SINTATICA: sucesso. A entrada pertence a linguagem LCC-2026-1.
```

Se houver erro sintatico por entrada vazia na tabela:

```text
ERRO SINTATICO: entrada vazia na tabela de reconhecimento sintatico M[TERM_TAIL, print]
forma sentencial alpha: TERM_TAIL NUMEXPRESSION_TAIL REL_OPT ; STMT_LIST_TAIL } FUNC_LIST_TAIL
nao-terminal mais a esquerda de alpha: TERM_TAIL
token atual da entrada: print (lexema 'print', linha 5, coluna 1)
```

Se ocorrer desencontro direto de terminal, a mensagem tambem informa a forma sentencial, o simbolo relevante e o token atual.

## 12. Gramatica final usada

A gramatica abaixo e a conversao da BNF original para forma convencional. Opcionais, repeticoes e alternativas foram expandidos por novos nao-terminais; isso nao restringe nem amplia os comandos da BNF original.

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

## 13. Condicoes relevantes do enunciado contempladas

- Existem tres programas `.lcc` validos.
- Cada programa valido possui pelo menos 100 linhas.
- Ha `Makefile` funcional.
- Ha `README.md`.
- O analisador lexico le caractere por caractere.
- Ha tabela de simbolos para identificadores.
- Ha deteccao de erro lexico com linha e coluna.
- Ha construcao de FIRST e FOLLOW.
- Ha construcao unica da tabela de reconhecimento.
- O parser usa efetivamente a tabela.
- Ha deteccao de erro sintatico com entrada da tabela, forma sentencial, nao-terminal e token atual.
- O projeto compila com `g++` em ambiente Linux/Unix.