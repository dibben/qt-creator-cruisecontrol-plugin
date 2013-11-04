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

#ifndef DATAFETCHER_H
#define DATAFETCHER_H

#include <QObject>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QNetworkAccessManager>

namespace CruiseControl {
namespace Internal {

class DataFetcher : public QObject
{
Q_OBJECT
public:
	DataFetcher(int maxItems, QObject *parent = 0);

signals:

	void projectItemReady(const QString& title, const QString& date, bool passed, const QString& url);

public slots:
	void fetch(const QUrl &url);
	void finished(int id, bool error);
	void readData(QNetworkReply*);

signals:
	void finished(bool error);

private:
	void parseXml();

	QXmlStreamReader m_xml;
	QString m_currentTag;
	QString m_linkString;
	QString m_titleString;
	QString m_dateString;
	bool m_passed;

	QNetworkAccessManager m_http;
	int m_items;
	int m_maxItems;

};

} // namespace Internal
} // namespace CruiseControl

#endif // DATAFETCHER_H
