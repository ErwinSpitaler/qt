/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QXmlContentHandler>

#include "Global.h"
#include "XMLWriter.h"

#include "TestSuiteResult.h"

using namespace QPatternistSDK;

TestSuiteResult::TestSuiteResult(const QString &testSuiteVersion,
                                 const QDate &runDate,
                                 const TestResult::List &results) : m_testSuiteVersion(testSuiteVersion),
                                                                    m_runDate(runDate),
                                                                    m_results(results)
{
}

TestSuiteResult::~TestSuiteResult()
{
    qDeleteAll(m_results);
}

void TestSuiteResult::toXML(XMLWriter &receiver) const
{
    /* If this data needs to be configurable in someway(say, another
     * XML format is supported), then break out the info into getters(alternatively, combined
     * with setters, or that the class is subclassed), and access the getters instead.
     */
    const QString organizationName          (QLatin1String("K Desktop Environment(KDE)"));
    const QString organizationWebsite       (QLatin1String("http://www.kde.org/"));
    const QString submittorName             (QLatin1String("Frans Englich"));
    const QString submittorEmail            (QLatin1String("frans.englich@nokia.com"));
    const QString implementationVersion     (QLatin1String("0.1"));
    const QString implementationName        (QLatin1String("Patternist"));
    const QString implementationDescription (QLatin1String(
                                             "Patternist is an implementation written in C++ "
                                             "and with the Qt/KDE libraries. "
                                             "It is licensed under GNU LGPL and part of KDE, "
                                             "the K Desktop Environment."));

    /* Not currently serialized:
     * - <implementation-defined-items>
     * - <features>
     * - <context-properties>
     */

    receiver.startDocument();
    /* <test-suite-result> */
    receiver.startPrefixMapping(QString(), Global::xqtsResultNS);
    receiver.startElement(QLatin1String("test-suite-result"));
    receiver.endPrefixMapping(QString());

    /* <implementation> */
    QXmlAttributes implementationAtts;
    implementationAtts.append(QLatin1String("name"), QString(),
                              QLatin1String("name"), implementationName);
    implementationAtts.append(QLatin1String("version"), QString(),
                              QLatin1String("version"), implementationVersion);
    receiver.startElement(QLatin1String("implementation"), implementationAtts);

    /* <organization> */
    QXmlAttributes organizationAtts;
    organizationAtts.append(QLatin1String("name"), QString(),
                            QLatin1String("name"), organizationName);
    organizationAtts.append(QLatin1String("website"), QString(),
                            QLatin1String("website"), organizationWebsite);
    receiver.startElement(QLatin1String("organization"), organizationAtts);

    /* </organization> */
    receiver.endElement(QLatin1String("organization"));

    /* <submittor> */
    QXmlAttributes submittorAtts;
    submittorAtts.append(QLatin1String("name"), QString(), QLatin1String("name"), submittorName);
    submittorAtts.append(QLatin1String("email"), QString(), QLatin1String("email"), submittorEmail);
    receiver.startElement(QLatin1String("submittor"), submittorAtts);

    /* </submittor> */
    receiver.endElement(QLatin1String("submittor"));

    /* <description> */
    receiver.startElement(QLatin1String("description"));

    /* <p> */
    receiver.startElement(QLatin1String("p"));
    receiver.characters(implementationDescription);

    /* </p> */
    receiver.endElement(QLatin1String("p"));
    /* </description> */
    receiver.endElement(QLatin1String("description"));

    /* </implementation> */
    receiver.endElement(QLatin1String("implementation"));

    /* <syntax> */
    receiver.startElement(QLatin1String("syntax"));
    receiver.characters(QLatin1String(QLatin1String("XQuery")));

    /* </syntax> */
    receiver.endElement(QLatin1String("syntax"));

    /* <test-run> */
    QXmlAttributes test_runAtts;
    test_runAtts.append(QLatin1String("dateRun"), QString(), QLatin1String("dateRun"), m_runDate.toString(QLatin1String("yyyy-MM-dd")));
    receiver.startElement(QLatin1String("test-run"), test_runAtts);

    /* <test-suite> */
    QXmlAttributes test_suiteAtts;
    test_suiteAtts.append(QLatin1String("version"), QString(), QLatin1String("version"), m_testSuiteVersion);
    receiver.startElement(QLatin1String("test-suite"), test_suiteAtts);

    /* </test-suite> */
    receiver.endElement(QLatin1String("test-suite"));

    /* </test-run> */
    receiver.endElement(QLatin1String("test-run"));

    /* Serialize the TestResults: tons of test-case elements. */
    const TestResult::List::const_iterator end(m_results.constEnd());
    TestResult::List::const_iterator it(m_results.constBegin());

    for(; it != end; ++it)
        (*it)->toXML(receiver);

    /* </test-suite-result> */
    receiver.endElement(QLatin1String("test-suite-result"));
    receiver.endDocument();
}

// vim: et:ts=4:sw=4:sts=4

