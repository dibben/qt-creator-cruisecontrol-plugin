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
