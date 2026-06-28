#ifndef LCC_TOKEN_HPP
#define LCC_TOKEN_HPP

#include <string>
#include <unordered_map>

/*
 * INE5622 - Introducao a Compiladores
 * EP - Analisador Lexico e Sintatico para LCC-2026-1
 * Integrantes do grupo: Gustavo Pauli de Souza - 23205950, Maria Eduarda Betman Damazio Jaques - 23200514, Pedro Henrique Archer Dalsenter - 23200515, Rafael Ribeiro de Lima - 23203995
 *
 * Este arquivo declara os tipos de token reconhecidos pelo analisador lexico,
 * incluindo palavras reservadas, identificadores, constantes, operadores,
 * delimitadores e marcador de fim de arquivo.
 *
 * Tambem declara funcoes auxiliares para converter tokens em nomes legiveis
 * e em terminais usados pela gramatica LL(1).
 */

// Tipos de token produzidos pelo analisador lexico.
enum class TokenType {
    Def,
    Int,
    Float,
    String,
    Print,
    Read,
    Return,
    If,
    Else,
    For,
    Break,
    New,
    Null,
    Identifier,
    IntConstant,
    FloatConstant,
    StringConstant,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,
    EqualEqual,
    NotEqual,
    Assign,
    Semicolon,
    Comma,
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    EndOfFile
};

// Representa uma ocorrencia concreta de token, com lexema, linha e coluna.
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

const std::unordered_map<std::string, TokenType>& reserved_words();
std::string token_type_name(TokenType type);
std::string token_to_terminal(TokenType type);
std::string token_display(const Token& token);

#endif
