#ifndef LCC_GRAMMAR_HPP
#define LCC_GRAMMAR_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995
 *
 * Este arquivo declara a estrutura responsavel pela representacao da
 * gramatica da linguagem LCC-2026-1 usada pelo analisador sintatico LL(1).
 *
 * A classe Grammar armazena as producoes da gramatica, identifica simbolos
 * terminais e nao-terminais, calcula os conjuntos FIRST e FOLLOW e constroi
 * a tabela de reconhecimento sintatico utilizada pelo parser tabular.
 */

// Representa uma producao da gramatica: lhs -> rhs.
struct Production {
    int id;
    std::string lhs;
    std::vector<std::string> rhs;
};

class Grammar {
public:
    static const std::string EPSILON;
    static const std::string END_MARKER;

    Grammar();

    const std::string& start_symbol() const;
    bool is_terminal(const std::string& symbol) const;
    bool is_nonterminal(const std::string& symbol) const;

    const std::map<std::string, std::vector<Production>>& productions_by_lhs() const;
    const std::map<std::string, std::set<std::string>>& first_sets() const;
    const std::map<std::string, std::set<std::string>>& follow_sets() const;
    const std::map<std::string, std::map<std::string, Production>>& parse_table() const;

    std::string format_production(const Production& production) const;

private:
    std::string start_symbol_;
    std::vector<Production> productions_;
    std::map<std::string, std::vector<Production>> productions_by_lhs_;
    std::set<std::string> nonterminals_;
    std::set<std::string> terminals_;
    std::map<std::string, std::set<std::string>> first_sets_;
    std::map<std::string, std::set<std::string>> follow_sets_;
    std::map<std::string, std::map<std::string, Production>> parse_table_;

    void build_grammar();
    void add_production(const std::string& lhs, std::vector<std::string> rhs);
    void finalize_symbols();
    void compute_first_sets();
    void compute_follow_sets();
    void build_parse_table();

    // FIRST(alfa), em que alfa pode ter zero ou mais simbolos.
    std::set<std::string> first_of_sequence(const std::vector<std::string>& sequence) const;
};

#endif
