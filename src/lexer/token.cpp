
#include "lexer/token.hpp"

auto to_string(TokenKind const &token) -> std::string_view {
    switch (token) {
    case TokenKind::IntegerLiteral:
        return "integer literal";
    case TokenKind::EndOfFile:
        return "end of file";
    case TokenKind::KwVar:
        return "var";
    case TokenKind::Identifier:
        return "identifier";
    case TokenKind::TkInt:
        return "int";
    case TokenKind::TkUint:
        return "uint";
    case TokenKind::SpSemicolon:
        return ";";
    case TokenKind::SpColon:
        return ":";
    case TokenKind::OpAssign:
        return "=";
    case TokenKind::KwImport:
        return "import";
    default:
        TODO("%d", token);
    }
}
