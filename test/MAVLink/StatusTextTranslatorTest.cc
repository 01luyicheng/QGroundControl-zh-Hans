#include "StatusTextTranslatorTest.h"

#include "StatusTextTranslator.h"

#include <QtCore/QLocale>

// The translator only applies below the locale gate, so every test runs with an
// explicitly installed chinese locale; cleanup restores the default (system
// following) locale the app installs before setLanguage() runs.
void StatusTextTranslatorTest::init()
{
    StatusTextTranslator::setLocale(QLocale(QLocale::Chinese, QLocale::China));
}

void StatusTextTranslatorTest::cleanup()
{
    StatusTextTranslator::setLocale(QLocale());
}

void StatusTextTranslatorTest::_testLocaleGate()
{
    StatusTextTranslator::setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("PreArm: No SD card")),
             QStringLiteral("PreArm: No SD card"));
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Arming denied! Home position invalid"), true),
             QStringLiteral("Arming denied! Home position invalid"));
    QCOMPARE(StatusTextTranslator::translate(QString()), QString());
}

void StatusTextTranslatorTest::_testArduPilotExactMatch()
{
    // The single-argument overload keeps matching the ArduPilot table only
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("PreArm: No SD card")),
             QStringLiteral("PreArm: 未检测到 SD 卡"));
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("PreArm: No SD card"), false),
             QStringLiteral("PreArm: 未检测到 SD 卡"));
}

void StatusTextTranslatorTest::_testArduPilotRegexMatch()
{
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("PreArm: Compass 1 not healthy")),
             QStringLiteral("PreArm: 罗盘 1 不健康"));
}

void StatusTextTranslatorTest::_testArduPilotPrefixSwap()
{
    // The same check failure under the other AP_Arming prefix tokens keeps the raw prefix
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Pre-Arm: Gyros not calibrated")),
             QStringLiteral("Pre-Arm: 陀螺仪未校准"));
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Arm: Gyros not calibrated")),
             QStringLiteral("Arm: 陀螺仪未校准"));
}

void StatusTextTranslatorTest::_testPx4ExactMatch()
{
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Arming denied! Home position invalid"), true),
             QStringLiteral("Arming denied! Home 点无效"));
}

void StatusTextTranslatorTest::_testPx4RegexMatch()
{
    // PX4 arming denials are matched as-is - no AP_Arming prefix swap on the PX4 table
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Preflight Fail: Compass 1 uncalibrated"), true),
             QStringLiteral("Preflight Fail: 罗盘 1 未校准"));
}

void StatusTextTranslatorTest::_testPx4UnknownPassesThrough()
{
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Some unknown px4 message"), true),
             QStringLiteral("Some unknown px4 message"));
}

void StatusTextTranslatorTest::_testTablesAreIsolated()
{
    // A PX4-only text passes through when the ArduPilot table is selected, and the
    // other way around - the tables never see each other's entries
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Arming denied! Home position invalid")),
             QStringLiteral("Arming denied! Home position invalid"));
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("Arming denied! Home position invalid"), false),
             QStringLiteral("Arming denied! Home position invalid"));
    QCOMPARE(StatusTextTranslator::translate(QStringLiteral("PreArm: No SD card"), true),
             QStringLiteral("PreArm: No SD card"));
}

UT_REGISTER_TEST(StatusTextTranslatorTest, TestLabel::Unit)
