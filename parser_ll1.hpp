#ifndef LCC_PARSER_LL1_HPP
#define LCC_PARSER_LL1_HPP

#include "grammar.hpp"
#include "token.hpp"

#include <string>
#include <vector>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995
 *
 * Este arquivo declara o analisador sintatico preditivo tabular LL(1).
 * O parser recebe a lista de tokens produzida pelo lexer, utiliza uma pilha
 * de simbolos e consulta a tabela de reconhecimento M[A,a] construida pela
 * classe Grammar.
 *
 * Em caso de erro, as funcoes auxiliares montam a mensagem com a forma
 * sentencial, o nao-terminal analisado e o token atual da entrada.
 */

class ParserLL1 {
public:
    ParserLL1();
    void parse(const std::vector<Token>& tokens) const;

private:
    // A gramatica mantem as producoes, FIRST, FOLLOW e a tabela LL(1).
    Grammar grammar_;

    static std::string stack_to_alpha(const std::vector<std::string>& stack);
    std::string leftmost_nonterminal(const std::vector<std::string>& stack) const;
    static std::string current_token_description(const Token& token);
    [[noreturn]] void throw_empty_table_error(
        const std::string& nonterminal,
        const Token& current,
        const std::vector<std::string>& stack) const;
    [[noreturn]] void throw_terminal_mismatch_error(
        const std::string& expected,
        const Token& current,
        const std::vector<std::string>& stack) const;
};

#endif
