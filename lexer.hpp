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
 * Analisador lexico implementado com leitura caractere por caractere.
 * Os metodos de reconhecimento correspondem aos diagramas de transicao
 * para identificadores/palavras reservadas, numeros, strings, operadores,
 * delimitadores e comentarios.
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

    // current_ guarda sempre o caractere atual; advance() consome um caractere.
    void advance();
    int peek();
    bool at_end() const;

    void skip_whitespace_and_comments();
    Token make_token(TokenType type, const std::string& lexeme, int line, int column) const;

    // Scanners especificos tornam explicitos os diagramas de transicao lexicos.
    Token scan_identifier_or_keyword();
    Token scan_number();
    Token scan_string();
    Token scan_operator_or_delimiter();
};

#endif
