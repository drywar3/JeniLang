#pragma once

#include "Lexer/SourceLocation.hpp"
#include "Common/this.hpp"
#include <string_view>
#include <string>
#include <vector>

struct Label {
  public:
    Label(std::string const &message, SourceLocation location,
          bool is_primary = false);

    auto Message(this Label const &self) -> std::string_view {
        return self.m_message;
    }
    auto Location(this Label const &self) { return self.m_location; }
    auto IsPrimary(this Label const &self) { return self.m_isPrimary; }

  private:
    std::string m_message;
    SourceLocation m_location;
    bool m_isPrimary;
};

struct Suggestion {
  public:
    enum Kind {
        Replace,
        Insert,
        Remove,
    };

    Suggestion(Kind kind, std::string const &message, SourceLocation location);

    auto GetKind(this const Suggestion &self) { return self.m_kind; }
    auto GetToken(this const Suggestion &self) { return self.m_token.subview(); }
    auto GetLocation(this const Suggestion &self) { return self.m_location; }

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

    auto WithLabel(this Diagnostic &self, Label const &label) -> Diagnostic &;
    auto WithMessage(this Diagnostic &self, std::string const &message)
        -> Diagnostic &;
    auto Suggest(this Diagnostic &self, Suggestion const &suggestion)
        -> Diagnostic &;

    auto GetSeverity(this Diagnostic const &self) { return self.m_severity; }

  private:
    Severity m_severity;
    std::string m_message;
    std::vector<Label> m_labels;
    std::vector<Suggestion> m_suggestions;
};

struct DiagnosticPool {
  public:
    explicit DiagnosticPool(usize, usize);
    auto Emit(this DiagnosticPool &self, Diagnostic const &diag) -> void;

  private:
    std::vector<Diagnostic> m_diagnostics;
    usize m_errorMax, m_warningMax;
    usize m_errorCount, m_warningCount;
};
