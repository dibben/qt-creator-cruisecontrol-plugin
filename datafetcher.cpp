/**************************************************************************
**
** Copyright (c) 2013 David Dibben
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#include "datafetcher.h"


#include <QDebug>
#include <QSysInfo>
#include <QLocale>
#include <QDesktopServices>
#include <QLineEdit>
#include <QNetworkProxyFactory>
#include <QNetworkReply>

#include <QFile>

#include <coreplugin/coreconstants.h>
#include <app/app_version.h>

#ifdef Q_OS_UNIX
#include <sys/utsname.h>
#endif

using namespace CruiseControl;
using namespace CruiseControl::Internal;


static const QString getOsString()
{
    QString osString;
#if defined(Q_OS_WIN)
    switch (QSysInfo::WindowsVersion) {
    case (QSysInfo::WV_4_0):
        osString += QLatin1String("WinNT4.0");
        break;
    case (QSysInfo::WV_5_0):
        osString += QLatin1String("Windows NT 5.0");
        break;
    case (QSysInfo::WV_5_1):
        osString += QLatin1String("Windows NT 5.1");
        break;
    case (QSysInfo::WV_5_2):
        osString += QLatin1String("Windows NT 5.2");
        break;
    case (QSysInfo::WV_6_0):
        osString += QLatin1String("Windows NT 6.0");
        break;
    case (QSysInfo::WV_6_1):
        osString += QLatin1String("Windows NT 6.1");
        break;
    default:
        osString += QLatin1String("Windows NT (Unknown)");
        break;
    }
#elif defined (Q_OS_MAC)
    if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
        osString += QLatin1String("PPC ");
    else
        osString += QLatin1String("Intel ");
    osString += QLatin1String("Mac OS X ");
    switch (QSysInfo::MacintoshVersion) {
    case (QSysInfo::MV_10_3):
        osString += QLatin1String("10_3");
        break;
    case (QSysInfo::MV_10_4):
        osString += QLatin1String("10_4");
        break;
    case (QSysInfo::MV_10_5):
        osString += QLatin1String("10_5");
        break;
    case (QSysInfo::MV_10_6):
        osString += QLatin1String("10_6");
        break;
    default:
        osString += QLatin1String("(Unknown)");
        break;
    }
#elif defined (Q_OS_UNIX)
    struct utsname uts;
    if (uname(&uts) == 0) {
        osString += QLatin1String(uts.sysname);
        osString += QLatin1String(" ");
        osString += QLatin1String(uts.release);
    } else {
        osString += QLatin1String("Unix (Unknown)");
    }
#else
    ossttring = QLatin1String("Unknown OS");
#endif
    return osString;
}

DataFetcher::DataFetcher(int maxItems, QObject *parent)
    : QObject(parent), m_items(0), m_maxItems(maxItems)
{
    connect(&m_http, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(readData(QNetworkReply*)));

}

void DataFetcher::fetch(const QUrl &url)
{
    m_xml.clear();

    if (!url.isValid()) {
        finished(0, true);
        return;
    }

    if (url.scheme() == QString::fromAscii("file")) {
        QString filename = url.toLocalFile();
        QFile file(filename);
        if (file.open(QFile::ReadOnly)) {
            m_xml.addData(file.readAll());
            parseXml();
            finished(0, false);
        } else {
            finished(0, true);
        }
        file.close();
        return;
    }


    QList<QNetworkProxy> proxies = QNetworkProxyFactory::systemProxyForQuery(QNetworkProxyQuery(url));
    if (proxies.count() > 0) {
        m_http.setProxy(proxies.first());
    }

    QNetworkRequest request;
    request.setUrl(url);
    QString agentStr = QString::fromAscii("Qt-Creator/%1 (QHttp %2; %3; %4; %5 bit)")
            .arg(QString::fromAscii(Core::Constants::IDE_VERSION_LONG)).arg(QString::fromAscii(qVersion()))
            .arg(getOsString()).arg(QLocale::system().name())
            .arg(QSysInfo::WordSize);
    //QHttpRequestHeader header(QString::fromAscii("GET"), url.path());
    //qDebug() << agentStr;
    request.setHeader(QNetworkRequest::UserAgentHeader, agentStr);
    //request.setRawHeader("Host", url.host().data());
    /*m_connectionId =*/ m_http.get(request);
}

void DataFetcher::readData(QNetworkReply* resp)
{
    if (resp->error() != QNetworkReply::NoError) {
        resp->abort();
        finished(0, true);
    }else {
        m_xml.addData(resp->readAll());
        parseXml();
        finished(0, false);
    }


}

void DataFetcher::finished(int id, bool error)
{
    Q_UNUSED(id)
    m_items = 0;
    m_xml.clear();
    emit finished(error);
}

/*
<item>
<title>connectfour passed Fri, 20 Nov 2009 09:50:12 +0900</title>
<description>Build passed</description>
<pubDate>Fri, 20 Nov 2009 09:50:12 +0900</pubDate>
<link>http://10.165.99.74:8080/cruisecontrol/buildresults/connectfour</link>
</item>

*/

void DataFetcher::parseXml()
{
    while (!m_xml.atEnd()) {
        m_xml.readNext();
        if (m_xml.isStartElement()) {
            if (m_xml.name() == QString::fromLatin1("item")) {
                m_titleString.clear();
                m_dateString.clear();
                m_linkString.clear();
                m_passed = true;
            }
            m_currentTag = m_xml.name().toString();
        } else if (m_xml.isEndElement()) {
            if (m_xml.name() == QString::fromLatin1("item")) {
                m_items++;
                if (m_items > m_maxItems)
                    return;
                emit projectItemReady(m_titleString, m_dateString, m_passed, m_linkString);
            }

        } else if (m_xml.isCharacters() && !m_xml.isWhitespace()) {
            if (m_currentTag == QString::fromAscii("title")) {
                QString title = m_xml.text().toString();
                int pos = title.toLower().indexOf(QString::fromAscii("passed"));
                if (pos < 0) {
                    pos = title.toLower().indexOf(QString::fromAscii("failed"));
                }
                m_titleString = (pos > 0 ? title.left(pos) : title).trimmed();
            } else if (m_currentTag == QString::fromAscii("pubDate")) {
                m_dateString += m_xml.text().toString();
            } else if (m_currentTag == QString::fromAscii("pubDate")) {
                m_dateString += m_xml.text().toString();
            } else if (m_currentTag == QString::fromAscii("description")) {
                m_passed = (m_xml.text().toString().toLower().indexOf(QString::fromAscii("passed")) > 0);
            }else if (m_currentTag == QString::fromAscii("link")) {
                m_linkString += m_xml.text().toString();
            }
        }
    }
    if (m_xml.error() && m_xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << m_xml.lineNumber() << ": " << m_xml.errorString();
        //m_http.abort();
    }
}
