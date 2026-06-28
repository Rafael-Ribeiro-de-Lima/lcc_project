#include "grammar.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

const std::string Grammar::EPSILON = "ε";
const std::string Grammar::END_MARKER = "$";

Grammar::Grammar()
    : start_symbol_("PROGRAM") {
    // Inicializa as estruturas da gramatica e constroi os dados usados pelo parser LL(1).
    build_grammar();
    finalize_symbols();
    compute_first_sets();
    compute_follow_sets();
    build_parse_table();
}

const std::string& Grammar::start_symbol() const {
    return start_symbol_;
}

bool Grammar::is_terminal(const std::string& symbol) const {
    return terminals_.find(symbol) != terminals_.end() || symbol == END_MARKER;
}

bool Grammar::is_nonterminal(const std::string& symbol) const {
    return nonterminals_.find(symbol) != nonterminals_.end();
}

const std::map<std::string, std::vector<Production>>& Grammar::productions_by_lhs() const {
    return productions_by_lhs_;
}

const std::map<std::string, std::set<std::string>>& Grammar::first_sets() const {
    return first_sets_;
}

const std::map<std::string, std::set<std::string>>& Grammar::follow_sets() const {
    return follow_sets_;
}

const std::map<std::string, std::map<std::string, Production>>& Grammar::parse_table() const {
    return parse_table_;
}

std::string Grammar::format_production(const Production& production) const {
    std::ostringstream oss;
    oss << production.lhs << " -> ";
    if (production.rhs.empty()) {
        oss << EPSILON;
    } else {
        for (std::size_t i = 0; i < production.rhs.size(); ++i) {
            if (i > 0) {
                oss << ' ';
            }
            oss << production.rhs[i];
        }
    }
    return oss.str();
}

// Registra a producao no vetor geral e no indice por lado esquerdo.
void Grammar::add_production(const std::string& lhs, std::vector<std::string> rhs) {
    const int id = static_cast<int>(productions_.size()) + 1;
    productions_.push_back(Production{id, lhs, std::move(rhs)});
    productions_by_lhs_[lhs].push_back(productions_.back());
    nonterminals_.insert(lhs);
}

void Grammar::build_grammar() {
    // Producao inicial da linguagem.
    add_production("PROGRAM", {"PROGRAM_OPT"});
    add_production("PROGRAM_OPT", {"FUNC_LIST"});
    add_production("PROGRAM_OPT", {"STATEMENT"});
    add_production("PROGRAM_OPT", {});

    // Lista de definicoes de funcoes.
    add_production("FUNC_LIST", {"FUNCDEF", "FUNC_LIST_TAIL"});
    add_production("FUNC_LIST_TAIL", {"FUNCDEF", "FUNC_LIST_TAIL"});
    add_production("FUNC_LIST_TAIL", {});

    // Definicao de funcao com parametros opcionais e bloco de comandos.
    add_production("FUNCDEF", {"def", "ident", "(", "PARAMS_OPT", ")", "{", "STMT_LIST", "}"});
    add_production("PARAMS_OPT", {"PARAM_LIST"});
    add_production("PARAMS_OPT", {});
    add_production("PARAM_LIST", {"TYPE", "ident", "PARAM_TAIL"});
    add_production("PARAM_TAIL", {",", "PARAM_LIST"});
    add_production("PARAM_TAIL", {});

    add_production("TYPE", {"int"});
    add_production("TYPE", {"float"});
    add_production("TYPE", {"string"});

    // Lista de comandos dentro de blocos e funcoes.
    add_production("STMT_LIST", {"STATEMENT", "STMT_LIST_TAIL"});
    add_production("STMT_LIST_TAIL", {"STATEMENT", "STMT_LIST_TAIL"});
    add_production("STMT_LIST_TAIL", {});

    // Alternativas de comandos reconhecidas pela linguagem.
    add_production("STATEMENT", {"VARDECL", ";"});
    add_production("STATEMENT", {"ATTRIBSTAT", ";"});
    add_production("STATEMENT", {"PRINTSTAT", ";"});
    add_production("STATEMENT", {"READSTAT", ";"});
    add_production("STATEMENT", {"RETURNSTAT", ";"});
    add_production("STATEMENT", {"IFSTAT"});
    add_production("STATEMENT", {"FORSTAT"});
    add_production("STATEMENT", {"{", "STMT_LIST", "}"});
    add_production("STATEMENT", {"break", ";"});
    add_production("STATEMENT", {";"});

    // Declaracao de variavel simples ou com dimensoes de vetor.
    add_production("VARDECL", {"TYPE", "ident", "ARRAY_DECL_LIST"});
    add_production("ARRAY_DECL_LIST", {"[", "int_constant", "]", "ARRAY_DECL_LIST"});
    add_production("ARRAY_DECL_LIST", {});

    // Atribuicao para expressao, alocacao ou chamada de funcao.
    add_production("ATTRIBSTAT", {"LVALUE", "=", "ATTRIB_RHS"});
    add_production("ATTRIB_RHS", {"ALLOCEXPRESSION"});
    add_production("ATTRIB_RHS", {"ident", "ATTRIB_RHS_IDENT_TAIL"});
    add_production("ATTRIB_RHS", {"NUMEXPRESSION_NON_IDENT_START", "REL_OPT"});

    // Tratamento do lado direito da atribuicao quando ele inicia por identificador.
    add_production("ATTRIB_RHS_IDENT_TAIL", {"(", "PARAMLIST_CALL_OPT", ")"});
    add_production("ATTRIB_RHS_IDENT_TAIL", {"INDEX_LIST", "TERM_TAIL", "NUMEXPRESSION_TAIL", "REL_OPT"});

    // Reconhecimento de expressoes numericas que nao iniciam por identificador.
    add_production("NUMEXPRESSION_NON_IDENT_START", {"TERM_NON_IDENT_START", "NUMEXPRESSION_TAIL"});
    add_production("TERM_NON_IDENT_START", {"UNARY_EXPR_NON_IDENT_START", "TERM_TAIL"});
    add_production("UNARY_EXPR_NON_IDENT_START", {"+", "FACTOR"});
    add_production("UNARY_EXPR_NON_IDENT_START", {"-", "FACTOR"});
    add_production("UNARY_EXPR_NON_IDENT_START", {"FACTOR_NON_IDENT"});
    add_production("FACTOR_NON_IDENT", {"int_constant"});
    add_production("FACTOR_NON_IDENT", {"float_constant"});
    add_production("FACTOR_NON_IDENT", {"string_constant"});
    add_production("FACTOR_NON_IDENT", {"null"});
    add_production("FACTOR_NON_IDENT", {"(", "NUMEXPRESSION", ")"});

    add_production("PARAMLIST_CALL_OPT", {"PARAMLIST_CALL"});
    add_production("PARAMLIST_CALL_OPT", {});
    add_production("PARAMLIST_CALL", {"ident", "PARAMLIST_CALL_TAIL"});
    add_production("PARAMLIST_CALL_TAIL", {",", "PARAMLIST_CALL"});
    add_production("PARAMLIST_CALL_TAIL", {});

    add_production("PRINTSTAT", {"print", "EXPRESSION"});
    add_production("READSTAT", {"read", "LVALUE"});
    add_production("RETURNSTAT", {"return"});

    // Comando condicional com parte else opcional.
    add_production("IFSTAT", {"if", "(", "EXPRESSION", ")", "STATEMENT", "ELSE_OPT"});
    add_production("ELSE_OPT", {"else", "STATEMENT"});
    add_production("ELSE_OPT", {});

    // Comando de repeticao for.
    add_production("FORSTAT", {"for", "(", "ATTRIBSTAT", ";", "EXPRESSION", ";", "ATTRIBSTAT", ")", "STATEMENT"});

    // Expressao de alocacao com uma ou mais dimensoes.
    add_production("ALLOCEXPRESSION", {"new", "TYPE", "ALLOC_DIMS"});
    add_production("ALLOC_DIMS", {"[", "NUMEXPRESSION", "]", "ALLOC_DIMS_TAIL"});
    add_production("ALLOC_DIMS_TAIL", {"[", "NUMEXPRESSION", "]", "ALLOC_DIMS_TAIL"});
    add_production("ALLOC_DIMS_TAIL", {});

    add_production("EXPRESSION", {"NUMEXPRESSION", "REL_OPT"});
    add_production("REL_OPT", {"REL_OP", "NUMEXPRESSION"});
    add_production("REL_OPT", {});
    add_production("REL_OP", {"<"});
    add_production("REL_OP", {">"});
    add_production("REL_OP", {"<="});
    add_production("REL_OP", {">="});
    add_production("REL_OP", {"=="});
    add_production("REL_OP", {"!="});

    add_production("NUMEXPRESSION", {"TERM", "NUMEXPRESSION_TAIL"});
    add_production("NUMEXPRESSION_TAIL", {"+", "TERM", "NUMEXPRESSION_TAIL"});
    add_production("NUMEXPRESSION_TAIL", {"-", "TERM", "NUMEXPRESSION_TAIL"});
    add_production("NUMEXPRESSION_TAIL", {});

    add_production("TERM", {"UNARY_EXPR", "TERM_TAIL"});
    add_production("TERM_TAIL", {"*", "UNARY_EXPR", "TERM_TAIL"});
    add_production("TERM_TAIL", {"/", "UNARY_EXPR", "TERM_TAIL"});
    add_production("TERM_TAIL", {"%", "UNARY_EXPR", "TERM_TAIL"});
    add_production("TERM_TAIL", {});

    add_production("UNARY_EXPR", {"+", "FACTOR"});
    add_production("UNARY_EXPR", {"-", "FACTOR"});
    add_production("UNARY_EXPR", {"FACTOR"});

    // Fatores aceitos em expressoes numericas.
    add_production("FACTOR", {"int_constant"});
    add_production("FACTOR", {"float_constant"});
    add_production("FACTOR", {"string_constant"});
    add_production("FACTOR", {"null"});
    add_production("FACTOR", {"LVALUE"});
    add_production("FACTOR", {"(", "NUMEXPRESSION", ")"});

    add_production("LVALUE", {"ident", "INDEX_LIST"});
    add_production("INDEX_LIST", {"[", "NUMEXPRESSION", "]", "INDEX_LIST"});
    add_production("INDEX_LIST", {});
}

// Simbolos do lado direito que nao sao nao-terminais sao tratados como terminais.
void Grammar::finalize_symbols() {
    for (const auto& production : productions_) {
        for (const auto& symbol : production.rhs) {
            if (symbol != EPSILON && nonterminals_.find(symbol) == nonterminals_.end()) {
                terminals_.insert(symbol);
            }
        }
    }
    terminals_.insert(END_MARKER);
}

// Algoritmo iterativo: repete ate nenhum conjunto FIRST mudar.
void Grammar::compute_first_sets() {
    for (const auto& terminal : terminals_) {
        first_sets_[terminal].insert(terminal);
    }
    for (const auto& nonterminal : nonterminals_) {
        first_sets_[nonterminal];
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& production : productions_) {
            auto& first_lhs = first_sets_[production.lhs];
            if (production.rhs.empty()) {
                changed = first_lhs.insert(EPSILON).second || changed;
                continue;
            }

            bool all_can_derive_epsilon = true;
            for (const auto& symbol : production.rhs) {
                const auto& first_symbol = first_sets_[symbol];
                for (const auto& item : first_symbol) {
                    if (item != EPSILON) {
                        changed = first_lhs.insert(item).second || changed;
                    }
                }
                if (first_symbol.find(EPSILON) == first_symbol.end()) {
                    all_can_derive_epsilon = false;
                    break;
                }
            }
            if (all_can_derive_epsilon) {
                changed = first_lhs.insert(EPSILON).second || changed;
            }
        }
    }
}

std::set<std::string> Grammar::first_of_sequence(const std::vector<std::string>& sequence) const {
    std::set<std::string> result;
    if (sequence.empty()) {
        result.insert(EPSILON);
        return result;
    }

    bool all_can_derive_epsilon = true;
    for (const auto& symbol : sequence) {
        const auto it = first_sets_.find(symbol);
        if (it == first_sets_.end()) {
            throw std::runtime_error("Simbolo desconhecido em FIRST: " + symbol);
        }
        for (const auto& item : it->second) {
            if (item != EPSILON) {
                result.insert(item);
            }
        }
        if (it->second.find(EPSILON) == it->second.end()) {
            all_can_derive_epsilon = false;
            break;
        }
    }

    if (all_can_derive_epsilon) {
        result.insert(EPSILON);
    }
    return result;
}

void Grammar::compute_follow_sets() {
    for (const auto& nonterminal : nonterminals_) {
        follow_sets_[nonterminal];
    }
    follow_sets_[start_symbol_].insert(END_MARKER);

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& production : productions_) {
            const auto& rhs = production.rhs;
            for (std::size_t i = 0; i < rhs.size(); ++i) {
                const std::string& symbol = rhs[i];
                if (!is_nonterminal(symbol)) {
                    continue;
                }

                std::vector<std::string> beta(rhs.begin() + static_cast<long>(i) + 1, rhs.end());
                const auto first_beta = first_of_sequence(beta);
                for (const auto& item : first_beta) {
                    if (item != EPSILON) {
                        changed = follow_sets_[symbol].insert(item).second || changed;
                    }
                }

                if (beta.empty() || first_beta.find(EPSILON) != first_beta.end()) {
                    for (const auto& item : follow_sets_[production.lhs]) {
                        changed = follow_sets_[symbol].insert(item).second || changed;
                    }
                }
            }
        }
    }
}

void Grammar::build_parse_table() {
    for (const auto& production : productions_) {
        const auto first_alpha = first_of_sequence(production.rhs);
        for (const auto& terminal : first_alpha) {
            if (terminal == EPSILON) {
                continue;
            }
            auto& cell = parse_table_[production.lhs][terminal];
            if (!cell.lhs.empty()) {
                throw std::runtime_error("Conflito LL(1) em M[" + production.lhs + ", " + terminal + "] entre " +
                                         format_production(cell) + " e " + format_production(production));
            }
            cell = production;
        }

        if (first_alpha.find(EPSILON) != first_alpha.end()) {
            for (const auto& terminal : follow_sets_[production.lhs]) {
                auto& cell = parse_table_[production.lhs][terminal];

                // Preserva a entrada ja existente quando ela deve ter prioridade sobre a producao vazia.
                if (production.lhs == "ELSE_OPT" && terminal == "else" && !cell.lhs.empty()) {
                    continue;
                }

                if (!cell.lhs.empty()) {
                    throw std::runtime_error("Conflito LL(1) em M[" + production.lhs + ", " + terminal + "] entre " +
                                             format_production(cell) + " e " + format_production(production));
                }
                cell = production;
            }
        }
    }
}
