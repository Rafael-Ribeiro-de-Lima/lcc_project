#include "token.hpp"

#include <sstream>

// Palavras reservadas sao reconhecidas antes de classificar como identificador.
const std::unordered_map<std::string, TokenType>& reserved_words() {
    static const std::unordered_map<std::string, TokenType> words = {
        {"def", TokenType::Def},
        {"int", TokenType::Int},
        {"float", TokenType::Float},
        {"string", TokenType::String},
        {"print", TokenType::Print},
        {"read", TokenType::Read},
        {"return", TokenType::Return},
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"for", TokenType::For},
        {"break", TokenType::Break},
        {"new", TokenType::New},
        {"null", TokenType::Null}
    };
    return words;
}

// Nome usado tanto na impressao dos tokens quanto como terminal da gramatica.
std::string token_type_name(TokenType type) {
    switch (type) {
        case TokenType::Def: return "def";
        case TokenType::Int: return "int";
        case TokenType::Float: return "float";
        case TokenType::String: return "string";
        case TokenType::Print: return "print";
        case TokenType::Read: return "read";
        case TokenType::Return: return "return";
        case TokenType::If: return "if";
        case TokenType::Else: return "else";
        case TokenType::For: return "for";
        case TokenType::Break: return "break";
        case TokenType::New: return "new";
        case TokenType::Null: return "null";
        case TokenType::Identifier: return "ident";
        case TokenType::IntConstant: return "int_constant";
        case TokenType::FloatConstant: return "float_constant";
        case TokenType::StringConstant: return "string_constant";
        case TokenType::Plus: return "+";
        case TokenType::Minus: return "-";
        case TokenType::Star: return "*";
        case TokenType::Slash: return "/";
        case TokenType::Percent: return "%";
        case TokenType::Less: return "<";
        case TokenType::Greater: return ">";
        case TokenType::LessEqual: return "<=";
        case TokenType::GreaterEqual: return ">=";
        case TokenType::EqualEqual: return "==";
        case TokenType::NotEqual: return "!=";
        case TokenType::Assign: return "=";
        case TokenType::Semicolon: return ";";
        case TokenType::Comma: return ",";
        case TokenType::LParen: return "(";
        case TokenType::RParen: return ")";
        case TokenType::LBrace: return "{";
        case TokenType::RBrace: return "}";
        case TokenType::LBracket: return "[";
        case TokenType::RBracket: return "]";
        case TokenType::EndOfFile: return "$";
    }
    return "<unknown>";
}

std::string token_to_terminal(TokenType type) {
    return token_type_name(type);
}

// Tokens nao triviais exibem tambem o lexema concreto encontrado.
std::string token_display(const Token& token) {
    const std::string name = token_type_name(token.type);
    if (token.type == TokenType::Identifier ||
        token.type == TokenType::IntConstant ||
        token.type == TokenType::FloatConstant ||
        token.type == TokenType::StringConstant) {
        std::ostringstream oss;
        oss << name << "(" << token.lexeme << ")";
        return oss.str();
    }
    return name;
}
