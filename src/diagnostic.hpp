#pragma once

#include "lexer/source_location.hpp"
#include "common/common.hpp"
#include <string_view>
#include <string>
#include <vector>

struct Label {
  public:
    Label(std::string const &message, SourceLocation location,
          bool is_primary = false);

    auto message(this Label const &self) -> std::string_view {
        return self.m_message;
    }
    auto location(this Label const &self) { return self.m_location; }
    auto is_primary(this Label const &self) { return self.m_is_primary; }

  private:
    std::string m_message;
    SourceLocation m_location;
    bool m_is_primary;
};

struct Suggestion {
  public:
    enum Kind {
        Replace,
        Insert,
        Remove,
    };

    Suggestion(Kind kind, std::string const &message, SourceLocation location);

    auto kind(this const Suggestion &self) { return self.m_kind; }
    auto token(this const Suggestion &self) { return self.m_token.subview(); }
    auto location(this const Suggestion &self) { return self.m_location; }

  private:
    Kind m_kind;
    std::string m_token;
    SourceLocation m_location;
};

enum struct Severity {
    Error,
    Warning,
    Note,
};

struct Diagnostic {
  public:
    Diagnostic(Severity sev, std::string const &message,
               std::string const &label, SourceLocation location);

    auto label(this Diagnostic &self, Label const &label) -> Diagnostic &;
    auto message(this Diagnostic &self, std::string const &message)
        -> Diagnostic &;
    auto suggest(this Diagnostic &self, Suggestion const &suggestion)
        -> Diagnostic &;

    auto severity(this Diagnostic const &self) { return self.m_severity; }

  private:
    Severity m_severity;
    std::string m_message;
    std::vector<Label> m_labels;
    std::vector<Suggestion> m_suggestions;
};

struct DiagnosticPool {
  public:
    explicit DiagnosticPool(usize, usize);
    auto emit(this DiagnosticPool &self, Diagnostic const &diag) -> void;

  private:
    std::vector<Diagnostic> m_diagnostics;
    usize error_max, warning_max;
    usize error_count, warning_count;
};
