#include "Diagnostic.hpp"
#include "Lexer/SourceLocation.hpp"
#include <string>

DiagnosticPool::DiagnosticPool(usize emax, usize wmax)
    : m_diagnostics(), m_errorMax(emax), m_warningMax(wmax), m_errorCount(0),
      m_warningCount(0) {}

auto DiagnosticPool::Emit(this DiagnosticPool &self, Diagnostic const &diag)
    -> void {
    if (self.m_errorCount >= self.m_errorMax &&
        diag.GetSeverity() == Severity::Error)
        return;

    if (self.m_warningCount >= self.m_warningMax &&
        diag.GetSeverity() == Severity::Warning)
        return;

    if (diag.GetSeverity() == Severity::Error)
        self.m_errorCount += 1;
    if (diag.GetSeverity() == Severity::Warning)
        self.m_warningCount += 1;

    self.m_diagnostics.push_back(std::move(diag));
}

Label::Label(std::string const &label, SourceLocation location, bool isPrimary) 
    : m_message(label), m_location(location), m_isPrimary(isPrimary) {

}

Diagnostic::Diagnostic(Severity sev, std::string const &message,
               std::string const &label, SourceLocation location) 
    : m_severity(sev), m_message(std::move(message)), m_labels(), m_suggestions() {
    m_labels.push_back(Label(label, location, true));
}