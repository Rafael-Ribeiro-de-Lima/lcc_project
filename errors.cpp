#include "errors.hpp"

#include <sstream>

// Monta a mensagem no formato exigido: erro simples com linha e coluna.
LexicalError::LexicalError(int line, int column, const std::string& message)
    : std::runtime_error([&]() {
          std::ostringstream oss;
          oss << "ERRO LEXICO: linha " << line << ", coluna " << column << ": " << message;
          return oss.str();
      }()),
      line_(line),
      column_(column) {}

int LexicalError::line() const noexcept {
    return line_;
}

int LexicalError::column() const noexcept {
    return column_;
}

// O parser ja monta a mensagem completa antes de lancar a excecao.
SyntaxError::SyntaxError(const std::string& message)
    : std::runtime_error(message) {}
