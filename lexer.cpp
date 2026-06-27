#include "lexer.hpp"

#include "errors.hpp"

#include <cctype>
#include <sstream>

Lexer::Lexer(std::istream& input)
    : input_(input), line_(1), column_(0), current_(0) {
    // Carrega o primeiro caractere e inicializa linha/coluna.
    advance();
}

// Percorre o arquivo inteiro e delega cada padrao lexical ao scanner adequado.
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!at_end()) {
        skip_whitespace_and_comments();
        if (at_end()) {
            break;
        }

        if (std::isalpha(current_) || current_ == '_') {
            tokens.push_back(scan_identifier_or_keyword());
        } else if (std::isdigit(current_)) {
            tokens.push_back(scan_number());
        } else if (current_ == '"') {
            tokens.push_back(scan_string());
        } else {
            tokens.push_back(scan_operator_or_delimiter());
        }
    }
    tokens.push_back(make_token(TokenType::EndOfFile, "$", line_, column_ + 1));
    return tokens;
}

const SymbolTable& Lexer::symbol_table() const {
    return symbol_table_;
}

// Avanca exatamente um caractere, mantendo a posicao atual para mensagens de erro.
void Lexer::advance() {
    current_ = input_.get();
    if (current_ == '\n') {
        ++line_;
        column_ = 0;
    } else if (current_ != EOF) {
        ++column_;
    }
}

int Lexer::peek() {
    return input_.peek();
}

bool Lexer::at_end() const {
    return current_ == EOF;
}

// Espacos e comentarios nao geram tokens, mas atualizam linha/coluna normalmente.
void Lexer::skip_whitespace_and_comments() {
    bool again = true;
    while (again && !at_end()) {
        again = false;
        while (!at_end() && std::isspace(current_)) {
            advance();
        }

        if (current_ == '/' && peek() == '/') {
            while (!at_end() && current_ != '\n') {
                advance();
            }
            again = true;
        } else if (current_ == '/' && peek() == '*') {
            const int start_line = line_;
            const int start_column = column_;
            advance();
            advance();
            bool closed = false;
            while (!at_end()) {
                if (current_ == '*' && peek() == '/') {
                    advance();
                    advance();
                    closed = true;
                    break;
                }
                advance();
            }
            if (!closed) {
                throw LexicalError(start_line, start_column, "comentario de bloco nao terminado");
            }
            again = true;
        }
    }
}

Token Lexer::make_token(TokenType type, const std::string& lexeme, int line, int column) const {
    return Token{type, lexeme, line, column};
}

// Diagrama: letra/_ seguido de letras, digitos ou _. Depois verifica palavra reservada.
Token Lexer::scan_identifier_or_keyword() {
    const int start_line = line_;
    const int start_column = column_;
    std::string lexeme;
    while (!at_end() && (std::isalnum(current_) || current_ == '_')) {
        lexeme.push_back(static_cast<char>(current_));
        advance();
    }

    const auto& words = reserved_words();
    const auto it = words.find(lexeme);
    if (it != words.end()) {
        return make_token(it->second, lexeme, start_line, start_column);
    }

    symbol_table_.add_occurrence(lexeme, start_line, start_column);
    return make_token(TokenType::Identifier, lexeme, start_line, start_column);
}

// Diagrama de numeros: inteiro ou float com digitos obrigatorios apos o ponto.
Token Lexer::scan_number() {
    const int start_line = line_;
    const int start_column = column_;
    std::string lexeme;

    while (!at_end() && std::isdigit(current_)) {
        lexeme.push_back(static_cast<char>(current_));
        advance();
    }

    if (current_ == '.') {
        if (!std::isdigit(peek())) {
            throw LexicalError(line_, column_, "constante float malformada");
        }
        lexeme.push_back(static_cast<char>(current_));
        advance();
        while (!at_end() && std::isdigit(current_)) {
            lexeme.push_back(static_cast<char>(current_));
            advance();
        }
        return make_token(TokenType::FloatConstant, lexeme, start_line, start_column);
    }

    return make_token(TokenType::IntConstant, lexeme, start_line, start_column);
}

// Strings preservam as aspas no lexema e rejeitam quebra de linha sem fechamento.
Token Lexer::scan_string() {
    const int start_line = line_;
    const int start_column = column_;
    std::string lexeme;
    lexeme.push_back('"');
    advance();

    while (!at_end() && current_ != '"') {
        if (current_ == '\n') {
            throw LexicalError(start_line, start_column, "constante string nao terminada");
        }
        if (current_ == '\\') {
            lexeme.push_back(static_cast<char>(current_));
            advance();
            if (at_end() || current_ == '\n') {
                throw LexicalError(start_line, start_column, "escape invalido em string");
            }
            lexeme.push_back(static_cast<char>(current_));
            advance();
        } else {
            lexeme.push_back(static_cast<char>(current_));
            advance();
        }
    }

    if (at_end()) {
        throw LexicalError(start_line, start_column, "constante string nao terminada");
    }

    lexeme.push_back('"');
    advance();
    return make_token(TokenType::StringConstant, lexeme, start_line, start_column);
}

// Operadores compostos sao testados com lookahead simples.
Token Lexer::scan_operator_or_delimiter() {
    const int start_line = line_;
    const int start_column = column_;
    const char c = static_cast<char>(current_);

    switch (c) {
        case '+': advance(); return make_token(TokenType::Plus, "+", start_line, start_column);
        case '-': advance(); return make_token(TokenType::Minus, "-", start_line, start_column);
        case '*': advance(); return make_token(TokenType::Star, "*", start_line, start_column);
        case '/': advance(); return make_token(TokenType::Slash, "/", start_line, start_column);
        case '%': advance(); return make_token(TokenType::Percent, "%", start_line, start_column);
        case ';': advance(); return make_token(TokenType::Semicolon, ";", start_line, start_column);
        case ',': advance(); return make_token(TokenType::Comma, ",", start_line, start_column);
        case '(': advance(); return make_token(TokenType::LParen, "(", start_line, start_column);
        case ')': advance(); return make_token(TokenType::RParen, ")", start_line, start_column);
        case '{': advance(); return make_token(TokenType::LBrace, "{", start_line, start_column);
        case '}': advance(); return make_token(TokenType::RBrace, "}", start_line, start_column);
        case '[': advance(); return make_token(TokenType::LBracket, "[", start_line, start_column);
        case ']': advance(); return make_token(TokenType::RBracket, "]", start_line, start_column);
        case '<':
            advance();
            if (current_ == '=') {
                advance();
                return make_token(TokenType::LessEqual, "<=", start_line, start_column);
            }
            return make_token(TokenType::Less, "<", start_line, start_column);
        case '>':
            advance();
            if (current_ == '=') {
                advance();
                return make_token(TokenType::GreaterEqual, ">=", start_line, start_column);
            }
            return make_token(TokenType::Greater, ">", start_line, start_column);
        case '=':
            advance();
            if (current_ == '=') {
                advance();
                return make_token(TokenType::EqualEqual, "==", start_line, start_column);
            }
            return make_token(TokenType::Assign, "=", start_line, start_column);
        case '!':
            advance();
            if (current_ == '=') {
                advance();
                return make_token(TokenType::NotEqual, "!=", start_line, start_column);
            }
            throw LexicalError(start_line, start_column, "operador invalido: use !=");
        default: {
            std::ostringstream oss;
            oss << "caractere invalido '" << c << "'";
            throw LexicalError(start_line, start_column, oss.str());
        }
    }
}
