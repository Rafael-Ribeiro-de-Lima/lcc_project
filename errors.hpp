#ifndef LCC_ERRORS_HPP
#define LCC_ERRORS_HPP

#include <stdexcept>
#include <string>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995
 */

class LexicalError : public std::runtime_error {
public:
    LexicalError(int line, int column, const std::string& message);
    int line() const noexcept;
    int column() const noexcept;

private:
    int line_;
    int column_;
};

class SyntaxError : public std::runtime_error {
public:
    explicit SyntaxError(const std::string& message);
};

#endif
