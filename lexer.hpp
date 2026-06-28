#ifndef LCC_LEXER_HPP
#define LCC_LEXER_HPP

#include "symbol_table.hpp"
#include "token.hpp"

#include <istream>
#include <string>
#include <vector>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995
 *
 * Este arquivo declara o analisador lexico da linguagem LCC-2026-1.
 * O lexer percorre a entrada caractere por caractere, reconhece os tokens
 * da linguagem, controla linha e coluna e registra identificadores na
 * tabela de simbolos.
 *
 * Os metodos privados separam o reconhecimento de identificadores/palavras
 * reservadas, numeros, strings, operadores, delimitadores e comentarios.
 */

class Lexer {
public:
    explicit Lexer(std::istream& input);

    std::vector<Token> tokenize();
    const SymbolTable& symbol_table() const;

private:
    std::istream& input_;
    SymbolTable symbol_table_;
    int line_;
    int column_;
    int current_;

    // current_ armazena o caractere atual da entrada; advance() consome o proximo.
    void advance();
    int peek();
    bool at_end() const;

    void skip_whitespace_and_comments();
    Token make_token(TokenType type, const std::string& lexeme, int line, int column) const;

    // Rotinas especificas de reconhecimento dos principais grupos lexicos.
    Token scan_identifier_or_keyword();
    Token scan_number();
    Token scan_string();
    Token scan_operator_or_delimiter();
};

#endif
