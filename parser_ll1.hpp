#ifndef LCC_PARSER_LL1_HPP
#define LCC_PARSER_LL1_HPP

#include "grammar.hpp"
#include "token.hpp"

#include <string>
#include <vector>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: preencher antes da entrega.
 *
 * Parser sintatico preditivo tabular LL(1). Ele nao e recursivo descendente:
 * usa uma pilha, a tabela M[A,a] construida em Grammar e a lista de tokens
 * produzida pelo analisador lexico.
 */

class ParserLL1 {
public:
    ParserLL1();
    void parse(const std::vector<Token>& tokens) const;

private:
    // A gramatica contem FIRST, FOLLOW e a tabela M[A,a] ja construidos.
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
