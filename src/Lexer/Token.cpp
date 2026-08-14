
#include "Lexer/Token.hpp"


auto ToString(TokenKind const &token) -> std::string_view {
    switch (token) {
    // Keywords
    case TokenKind::KwStruct:    return "struct";
    case TokenKind::KwEnum:      return "enum";
    case TokenKind::KwReturn:    return "return";
    case TokenKind::KwUsing:     return "using";
    case TokenKind::KwImport:    return "import";
    case TokenKind::KwNamespace: return "namespace";
    case TokenKind::KwFunc:      return "func";
    case TokenKind::KwVar:       return "var";
    case TokenKind::KwConst:     return "const";
    case TokenKind::KwComptime:  return "comptime";
    case TokenKind::KwCpp:       return "cpp";
    case TokenKind::KwMove:      return "move";
    case TokenKind::KwOperator:  return "operator";
    case TokenKind::KwNew:       return "new";
    case TokenKind::KwDrop:      return "drop";
    case TokenKind::KwInterface: return "interface";
    case TokenKind::KwPublic:    return "public";
    case TokenKind::KwPrivate:   return "private";
    case TokenKind::KwShared:    return "shared";
    case TokenKind::KwFor:       return "for";
    case TokenKind::KwIn:        return "in";
    case TokenKind::KwIf:        return "if";
    case TokenKind::KwElse:      return "else";
    case TokenKind::KwWhile:     return "while";
    case TokenKind::KwLoop:      return "loop";
    case TokenKind::KwBreak:     return "break";
    case TokenKind::KwContinue:  return "continue";
    case TokenKind::KwType:      return "type";
    case TokenKind::KwRequires:  return "requires";
    case TokenKind::KwLabel:     return "label";
    case TokenKind::KwThrow:     return "throw";
    case TokenKind::KwThrows:    return "throws";
    case TokenKind::KwCatch:     return "catch";
    case TokenKind::KwPanic:     return "panic";
    case TokenKind::KwPanics:    return "panics";
    case TokenKind::KwTodo:      return "todo";
    case TokenKind::KwOrelse:    return "orelse";
    case TokenKind::KwNullable:  return "nullable";
    case TokenKind::KwKeep:      return "keep";
    case TokenKind::KwExtend:    return "extend";

    // Operators
    case TokenKind::OpAdd:        return "+";
    case TokenKind::OpSub:        return "-";
    case TokenKind::OpSlash:      return "/";
    case TokenKind::OpStar:       return "*";
    case TokenKind::OpAssign:     return "=";
    case TokenKind::OpEquals:     return "==";
    case TokenKind::OpNotEquals:  return "!=";
    case TokenKind::OpAmp:        return "&";
    case TokenKind::OpArrow:      return "->";

    // Special characters
    case TokenKind::SpSemicolon:     return ";";
    case TokenKind::SpColon:         return ":";
    case TokenKind::SpComma:         return ",";
    case TokenKind::SpDot:           return ".";
    case TokenKind::SpNamespace:     return "::";
    case TokenKind::SpOpenParen:     return "(";
    case TokenKind::SpOpenBrace:     return "{";
    case TokenKind::SpOpenBracket:   return "[";
    case TokenKind::SpCloseParen:    return ")";
    case TokenKind::SpCloseBrace:    return "}";
    case TokenKind::SpCloseBracket:  return "]";

    // Types
    case TokenKind::TkInt:      return "int";
    case TokenKind::TkUint:     return "uint";
    case TokenKind::TkInt8:     return "int8";
    case TokenKind::TkInt16:    return "int16";
    case TokenKind::TkInt32:    return "int32";
    case TokenKind::TkInt64:    return "int64";
    case TokenKind::TkUint8:    return "uint8";
    case TokenKind::TkUint16:   return "uint16";
    case TokenKind::TkUint32:   return "uint32";
    case TokenKind::TkUint64:   return "uint64";
    case TokenKind::TkUsize:    return "usize";
    case TokenKind::TkIsize:    return "isize";
    case TokenKind::TkChar:     return "char";
    case TokenKind::TkString:   return "string";
    case TokenKind::TkVoid:     return "void";
    case TokenKind::TkBool:     return "bool";
    case TokenKind::TkCallable: return "callable";

    // Identifiers and literals
    case TokenKind::Identifier:        return "identifier";
    case TokenKind::IntegerLiteral:   return "integer literal";
    case TokenKind::FloatLiteral:     return "float literal";
    case TokenKind::StringLiteral:    return "string literal";
    case TokenKind::CStringLiteral:   return "C string literal";
    case TokenKind::RawStringLiteral: return "raw string literal";
    case TokenKind::RawCStringLiteral:
        return "raw C string literal";

    case TokenKind::EndOfFile:
        return "end of file";

    default:
        TODO("%d", token);
    }
}
