#include "errors.hpp"
#include "lexer.hpp"
#include "parser_ll1.hpp"
#include "token.hpp"

#include <fstream>
#include <iostream>
#include <vector>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995
 *
 * Uso:
 *   ./lcc_analyzer caminho/do/arquivo.lcc
 */

// Imprime a lista lexical na mesma ordem em que os tokens aparecem no codigo fonte.
static void print_tokens(const std::vector<Token>& tokens) {
    std::cout << "LISTA DE TOKENS\n[";
    bool first = true;
    for (const Token& token : tokens) {
        if (token.type == TokenType::EndOfFile) {
            continue;
        }
        if (!first) {
            std::cout << ", ";
        }
        std::cout << token_display(token);
        first = false;
    }
    std::cout << "]\n\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo.lcc>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "ERRO: nao foi possivel abrir o arquivo: " << argv[1] << "\n";
        return 1;
    }

    try {
        // Pipeline da analise: primeiro tokeniza; se nao houver erro lexico, chama o parser.
        Lexer lexer(input);
        const std::vector<Token> tokens = lexer.tokenize();

        print_tokens(tokens);
        lexer.symbol_table().print(std::cout);
        std::cout << "\n";

        // O parser constroi a gramatica, FIRST, FOLLOW e tabela LL(1) uma unica vez.
        ParserLL1 parser;
        parser.parse(tokens);
        std::cout << "ANALISE SINTATICA: sucesso. A entrada pertence a linguagem LCC-2026-1.\n";
        return 0;
    } catch (const LexicalError& error) {
        std::cout << error.what() << "\n";
        return 1;
    } catch (const SyntaxError& error) {
        std::cout << error.what() << "\n";
        return 2;
    } catch (const std::exception& error) {
        std::cout << "ERRO INTERNO: " << error.what() << "\n";
        return 3;
    }
}
