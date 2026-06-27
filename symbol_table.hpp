#ifndef LCC_SYMBOL_TABLE_HPP
#define LCC_SYMBOL_TABLE_HPP

#include <map>
#include <ostream>
#include <string>
#include <vector>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: preencher antes da entrega.
 *
 * A tabela de simbolos possui uma entrada por identificador distinto e
 * armazena todas as ocorrencias como pares linha/coluna.
 */

// Posicao inicial de uma ocorrencia do identificador.
struct Occurrence {
    int line;
    int column;
};

class SymbolTable {
public:
    void add_occurrence(const std::string& identifier, int line, int column);
    const std::map<std::string, std::vector<Occurrence>>& entries() const;
    void print(std::ostream& out) const;

private:
    std::map<std::string, std::vector<Occurrence>> entries_;
};

#endif
