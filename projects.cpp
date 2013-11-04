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

#include "projects.h"

#include <QStringList>
#include <QDebug>

using namespace CruiseControl;
using namespace CruiseControl::Internal;

Projects::Projects()
{
	m_connectionError = false;
}

bool Projects::hasError() const
{
	foreach (const Project& p, m_list) {
		if (!p.passed) return true;
	}
	return false;
}

bool Projects::connectionError() const
{
	return m_connectionError;
}

int Projects::size() const
{
	return m_list.size();
}

QString Projects::name(int i) const
{
	if (i < 0  || i >= size()) return QString();
	return m_list[i].name;
}

QString Projects::date(int i) const
{
	if (i < 0  || i >= size()) return QString();
	return m_list[i].date;
}

bool Projects::passed(int i) const
{
	if (i < 0  || i >= size()) return false;
	return m_list[i].passed;
}

QString Projects::link(int i) const
{
	if (i < 0  || i >= size()) return QString();
	return m_list[i].link;
}

void Projects::setIgnored(const QString& list)
{
	m_ignored.clear();
    QStringList items = list.split(QString::fromAscii(","));
	foreach (QString it, items){
		it = it.trimmed();
		m_ignored.insert(it);
	}
}

void Projects::add(const QString& name, const QString& date, bool passed, const QString& link)
{
	if (m_ignored.contains(name)) return;

	Project rec;
	rec.name = name;
	rec.date = date;
	rec.passed = passed;
	rec.link = link;

	m_list.append(rec);

	emit projectsChanged();
}

void Projects::setConnectionError(bool error)
{
	m_connectionError = error;
	emit projectsChanged();
}

void Projects::clear()
{
	m_list.clear();
	m_connectionError = false;

	emit projectsChanged();
}


QString	Projects::resultsUrl() const
{
	if (size() == 0) return QString();

	QString linkstr = link(0);

	//drop last 2 items from url
    int pos = linkstr.lastIndexOf(QChar::fromAscii('/'));
	if (pos == -1) return QString();
    pos = linkstr.lastIndexOf(QChar::fromAscii('/'), pos - 1);
	if (pos == -1) return QString();
	return linkstr.left(pos);
}
