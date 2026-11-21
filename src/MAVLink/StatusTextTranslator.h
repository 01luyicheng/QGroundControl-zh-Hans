#pragma once

#include <QtCore/QString>

/// Friendly zh-Hans translation of firmware STATUSTEXT and event text messages.
///
/// Display layer only: the corpus is matched against the complete firmware text
/// (after chunked STATUSTEXT reassembly). Anything unmatched, or when the ui
/// language is not chinese, is returned unchanged - messages are never dropped.
class StatusTextTranslator
{
public:
    /// Translates a firmware status text for display. Returns the raw text on
    /// any mismatch, corpus load failure or error.
    static QString translate(const QString &raw);

private:
    StatusTextTranslator() = delete;
};
