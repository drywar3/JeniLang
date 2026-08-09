#include "diagnostic.hpp"

DiagnosticPool::DiagnosticPool(usize emax, usize wmax)
    : m_diagnostics(), error_max(emax), warning_max(wmax), error_count(0),
      warning_count(0) {}

auto DiagnosticPool::emit(this DiagnosticPool &self, Diagnostic const &diag)
    -> void {
    if (self.error_count >= self.error_max &&
        diag.severity() == Severity::Error)
        return;

    if (self.warning_count >= self.warning_max &&
        diag.severity() == Severity::Warning)
        return;

    if (diag.severity() == Severity::Error)
        self.error_count += 1;
    if (diag.severity() == Severity::Warning)
        self.warning_count += 1;

    self.m_diagnostics.push_back(std::move(diag));
}
