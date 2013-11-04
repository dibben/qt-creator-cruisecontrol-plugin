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

#ifndef PROJECTS_H
#define PROJECTS_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSet>

namespace CruiseControl {
namespace Internal {

struct Project
{
    QString	name;
    QString date;
    bool passed;
    QString link;
};

class Projects : public QObject
{
    Q_OBJECT
public:
    Projects();

    bool	hasError() const;
    bool	connectionError() const;

    int		size() const;
    QString	name(int i) const;
    QString	date(int i) const;
    bool	passed(int i) const;
    QString	link(int i) const;

    void	setIgnored(const QString& list);

    QString	resultsUrl() const;

public slots:

    void	setConnectionError(bool error);
    void	add(const QString& name, const QString& date, bool passed, const QString& link);
    void	clear();

signals:

    void	projectsChanged();

private:

    QList<Project>	m_list;
    bool			m_connectionError;
    QSet<QString>	m_ignored;
};

} // namespace Internal
} // namespace CruiseControl

#endif // PROJECTS_H
