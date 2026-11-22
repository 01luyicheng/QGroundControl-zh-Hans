#pragma once

#include <QtCore/QLocale>
#include <QtCore/QString>

/// Friendly zh-Hans translation of firmware STATUSTEXT and event text messages.
///
/// Display layer only: the corpus is matched against the complete firmware text
/// (after chunked STATUSTEXT reassembly). Anything unmatched, or when the ui
/// language is not chinese, is returned unchanged - messages are never dropped.
///
/// One corpus is kept per firmware family (ArduPilot, PX4); the tables are fully
/// independent - a text that only exists in one corpus is passed through when the
/// other firmware is selected.
class StatusTextTranslator
{
public:
    /// Installs the application ui locale. Called from QGCApplication::setLanguage().
    /// Checking this locale explicitly (instead of a default-constructed QLocale)
    /// is required because setLanguage() skips QLocale::setDefault() for en_US,
    /// which would otherwise leave the gate on the system language.
    static void setLocale(const QLocale &locale);

    /// Translates a firmware status text for display using the ArduPilot corpus.
    /// Returns the raw text on any mismatch, corpus load failure or error.
    static QString translate(const QString &raw);

    /// Translates a firmware status text or event text for display, selecting the
    /// corpus by firmware family. The PX4 corpus is matched without the AP_Arming
    /// "PreArm:"/"Pre-Arm:"/"Arm:" prefix swap - PX4 arming denials have no such
    /// prefix variants. Returns the raw text on any mismatch, corpus load failure
    /// or error.
    static QString translate(const QString &raw, bool isPx4);

private:
    StatusTextTranslator() = delete;
};
