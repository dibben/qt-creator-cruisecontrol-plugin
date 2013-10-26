#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include "projects.h"

#include <QWidget>
#include <QLabel>

namespace CruiseControl {
namespace Internal {

class DisplayWidget : public QWidget
{
Q_OBJECT
public:
	DisplayWidget(Projects* projects);


	QString	tooltipText() const;

signals:

	void	refreshRequested();
	void	doubleClicked();

protected:

	bool event(QEvent* event);
	void mouseDoubleClickEvent(QMouseEvent * event);

private slots:

	void	updateState();

private:
	QLabel *m_errorIcon;
	QLabel *m_passIcon;
	QLabel *m_noConnectionIcon;
	Projects* m_projects;
};

} // namespace Internal
} // namespace CruiseControl

#endif // DISPLAYWIDGET_H
