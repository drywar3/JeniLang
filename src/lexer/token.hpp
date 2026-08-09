#pragma once

#include "lexer/source_location.hpp"
#include <string_view>

enum struct TokenKind {
    KwStruct = 0,
    KwEnum,
    KwReturn,
    KwUsing,
    KwImport,
    KwNamespace,
    KwFunc,
    KwVar,
    KwConst,
    KwComptime,
    KwCpp,
    KwMove,
    KwOperator,
    KwNew,
    KwDrop,
    KwInterface,
    KwPublic,
    KwPrivate,
    KwShared,
    KwFor,
    KwIn,
    KwIf,
    KwElse,
    KwWhile,
    KwLoop,
    KwBreak,
    KwContinue,
    KwType,
    KwRequires,
    KwLabel,
    KwThrow,
    KwThrows,
    KwCatch,
    KwPanic,
    KwPanics,
    KwTodo,
    KwOrelse,
    KwNullable,
    KwKeep,
    KwExtend,

    OpAdd,
    OpSub,
    OpSlash,
    OpStar,
    OpAssign,
    OpEquals,
    OpNotEquals,
    OpAmp,   // &
    OpArrow, // ->

    SpSemicolon,
    SpColon,
    SpComma,
    SpDot,
    SpNamespace,
    SpOpenParen,
    SpOpenBrace,
    SpOpenBracket,
    SpCloseParen,
    SpCloseBrace,
    SpCloseBracket,

    TkInt,
    TkUint,
    TkInt8,
    TkInt16,
    TkInt32,
    TkInt64,
    TkUint8,
    TkUint16,
    TkUint32,
    TkUint64,
    TkUsize,
    TkIsize,
    TkChar,
    TkString,
    TkVoid,
    TkBool,
    TkCallable,

    Identifier,
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,
    CStringLiteral,
    RawStringLiteral,
    RawCStringLiteral,
    EndOfFile,
};

auto to_string(TokenKind const &token) -> std::string_view;

struct Token {
    TokenKind kind;
    SourceLocation location;

    Token(TokenKind kind, SourceLocation location)
        : kind(kind), location(location) {}
};
