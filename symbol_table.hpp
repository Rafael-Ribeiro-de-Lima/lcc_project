#ifndef LCC_SYMBOL_TABLE_HPP
#define LCC_SYMBOL_TABLE_HPP

#include <map>
#include <ostream>
#include <string>
#include <vector>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995
 *
 * Este arquivo declara a tabela de simbolos exigida pela analise lexica.
 * A tabela possui uma entrada para cada identificador distinto encontrado
 * no codigo-fonte e armazena todas as suas ocorrencias com linha e coluna.
 */

// Linha e coluna inicial de uma ocorrencia de identificador no arquivo de entrada.
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
