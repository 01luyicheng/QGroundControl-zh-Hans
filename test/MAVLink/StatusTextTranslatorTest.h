#pragma once

#include "UnitTest.h"

class StatusTextTranslatorTest : public UnitTest
{
    Q_OBJECT

private slots:
    void _testLocaleGate();
    void _testArduPilotExactMatch();
    void _testArduPilotRegexMatch();
    void _testArduPilotPrefixSwap();
    void _testPx4ExactMatch();
    void _testPx4RegexMatch();
    void _testPx4UnknownPassesThrough();
    void _testTablesAreIsolated();

protected:
    void init() override;
    void cleanup() override;
};
