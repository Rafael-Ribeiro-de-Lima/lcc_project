#include "symbol_table.hpp"

#include <iomanip>

// Cada identificador aponta para todas as posicoes em que apareceu no arquivo.
void SymbolTable::add_occurrence(const std::string& identifier, int line, int column) {
    entries_[identifier].push_back({line, column});
}

const std::map<std::string, std::vector<Occurrence>>& SymbolTable::entries() const {
    return entries_;
}

// Saida da tabela de simbolos em ordem alfabetica, pois std::map mantem ordenacao.
void SymbolTable::print(std::ostream& out) const {
    out << "TABELA DE SIMBOLOS\n";
    out << "Identificador | Ocorrencias (linha,coluna)\n";
    out << "------------------------------------------\n";
    for (const auto& item : entries_) {
        out << std::left << std::setw(13) << item.first << " | ";
        const auto& occurrences = item.second;
        for (std::size_t i = 0; i < occurrences.size(); ++i) {
            out << "(" << occurrences[i].line << "," << occurrences[i].column << ")";
            if (i + 1 < occurrences.size()) {
                out << ", ";
            }
        }
        out << "\n";
    }
}
