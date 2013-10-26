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
