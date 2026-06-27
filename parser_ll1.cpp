#include "parser_ll1.hpp"

#include "errors.hpp"

#include <sstream>

ParserLL1::ParserLL1()
    : grammar_() {}

// Algoritmo preditivo tabular: pilha + lookahead + tabela M[A,a].
void ParserLL1::parse(const std::vector<Token>& tokens) const {
    std::vector<std::string> stack;
    stack.push_back(Grammar::END_MARKER);
    stack.push_back(grammar_.start_symbol());

    std::size_t input_index = 0;
    while (!stack.empty()) {
        if (input_index >= tokens.size()) {
            throw SyntaxError("ERRO SINTATICO: fim inesperado da lista de tokens");
        }

        const std::string top = stack.back();
        const Token& current = tokens[input_index];
        const std::string lookahead = token_to_terminal(current.type);

        // Aceitacao: pilha e entrada chegam juntas ao marcador final.
        if (top == Grammar::END_MARKER && lookahead == Grammar::END_MARKER) {
            return;
        }

        // Terminal no topo deve casar exatamente com o token atual.
        if (grammar_.is_terminal(top) || top == Grammar::END_MARKER) {
            if (top == lookahead) {
        stack.pop_back();
                ++input_index;
            } else {
                throw_terminal_mismatch_error(top, current, stack);
            }
            continue;
        }

        // Nao-terminal no topo: consulta obrigatoria a M[top, lookahead].
        const auto table_row = grammar_.parse_table().find(top);
        if (table_row == grammar_.parse_table().end()) {
            throw_empty_table_error(top, current, stack);
        }

        const auto production_it = table_row->second.find(lookahead);
        if (production_it == table_row->second.end()) {
            throw_empty_table_error(top, current, stack);
        }

        const Production& production = production_it->second;
        stack.pop_back();
        // Empilha a producao ao contrario para manter a derivacao mais a esquerda.
        for (auto it = production.rhs.rbegin(); it != production.rhs.rend(); ++it) {
            if (!it->empty() && *it != Grammar::EPSILON) {
                stack.push_back(*it);
            }
        }
    }

    if (input_index != tokens.size()) {
        throw SyntaxError("ERRO SINTATICO: entrada restante apos esvaziamento da pilha");
    }
}

// A forma sentencial alpha corresponde ao conteudo atual da pilha, sem o marcador $.
std::string ParserLL1::stack_to_alpha(const std::vector<std::string>& stack) {
    std::ostringstream oss;
    bool first = true;
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        if (*it == Grammar::END_MARKER) {
            continue;
        }
        if (!first) {
            oss << ' ';
        }
        oss << *it;
        first = false;
    }
    if (first) {
        return Grammar::EPSILON;
    }
    return oss.str();
}

std::string ParserLL1::leftmost_nonterminal(const std::vector<std::string>& stack) const {
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        if (grammar_.is_nonterminal(*it)) {
            return *it;
        }
    }
    return "<nenhum>";
}

std::string ParserLL1::current_token_description(const Token& token) {
    std::ostringstream oss;
    oss << token_to_terminal(token.type) << " (lexema '" << token.lexeme
        << "', linha " << token.line << ", coluna " << token.column << ")";
    return oss.str();
}

// Mensagem exigida pelo enunciado para entrada vazia na tabela LL(1).
void ParserLL1::throw_empty_table_error(
    const std::string& nonterminal,
    const Token& current,
    const std::vector<std::string>& stack) const {
    std::ostringstream oss;
    oss << "ERRO SINTATICO: entrada vazia na tabela de reconhecimento sintatico M["
        << nonterminal << ", " << token_to_terminal(current.type) << "]\n"
        << "forma sentencial alpha: " << stack_to_alpha(stack) << "\n"
        << "nao-terminal mais a esquerda de alpha: " << nonterminal << "\n"
        << "token atual da entrada: " << current_token_description(current);
    throw SyntaxError(oss.str());
}

void ParserLL1::throw_terminal_mismatch_error(
    const std::string& expected,
    const Token& current,
    const std::vector<std::string>& stack) const {
    const std::string leftmost = leftmost_nonterminal(stack);
    std::ostringstream oss;
    oss << "ERRO SINTATICO: terminal inesperado; esperado '" << expected << "'\n"
        << "entrada vazia/indefinida na tabela de reconhecimento sintatico M["
        << leftmost << ", " << token_to_terminal(current.type) << "]\n"
        << "forma sentencial alpha: " << stack_to_alpha(stack) << "\n"
        << "nao-terminal mais a esquerda de alpha: " << leftmost << "\n"
        << "token atual da entrada: " << current_token_description(current);
    throw SyntaxError(oss.str());
}
