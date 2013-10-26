#include "displaywidget.h"

#include <utils/stylehelper.h>

#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QHelpEvent>
#include <QToolTip>
#include <QAction>

using namespace CruiseControl;
using namespace CruiseControl::Internal;



DisplayWidget::DisplayWidget(Projects* projects) :
	m_errorIcon(new QLabel(this)),
	m_passIcon(new QLabel(this)),
	m_noConnectionIcon(new QLabel(this)),
	m_projects(projects)
{

	setContextMenuPolicy(Qt::ActionsContextMenu);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(8, 2, 0, 2);
	layout->setSpacing(2);
	setLayout(layout);
	QHBoxLayout *errorLayout = new QHBoxLayout;
	errorLayout->setSpacing(2);
	layout->addLayout(errorLayout);
	errorLayout->addStretch(1);
	errorLayout->addWidget(m_errorIcon);
	errorLayout->addWidget(m_noConnectionIcon);
	errorLayout->addWidget(m_passIcon);
	errorLayout->addStretch(2);


	m_errorIcon->setAlignment(Qt::AlignCenter);
	m_passIcon->setAlignment(Qt::AlignLeft);

    m_errorIcon->setPixmap(QPixmap(QString::fromAscii(":/cruisecontrol/resources/red-ball.png")));
    m_noConnectionIcon->setPixmap(QPixmap(QString::fromAscii(":/cruisecontrol/resources/red-cross.png")));
    m_passIcon->setPixmap(QPixmap(QString::fromAscii(":/cruisecontrol/resources/green-ball.png")));

	connect(m_projects, SIGNAL(projectsChanged()), this, SLOT(updateState()));

	QAction* refresh = new QAction(tr("Refresh"),this);
	connect(refresh, SIGNAL(triggered()), this, SIGNAL(refreshRequested()));
	addAction(refresh);

	updateState();
}

void DisplayWidget::updateState()
{
	if (m_projects == 0) return;

	if (m_projects->connectionError()) {
		m_noConnectionIcon->setHidden(false);
		m_errorIcon->setHidden(true);
		m_passIcon->setHidden(true);
	} else {
		m_noConnectionIcon->setHidden(true);
		m_errorIcon->setHidden(false);
		m_passIcon->setHidden(false);
		bool error = m_projects->hasError();
		int numProjects = m_projects->size();
		m_passIcon->setEnabled(!error && numProjects > 0);
		m_errorIcon->setEnabled(error && numProjects > 0);
	}
}

bool DisplayWidget::event(QEvent* event)
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);

		QToolTip::showText(helpEvent->globalPos(), tooltipText());
		return true;
	}
	return QWidget::event(event);
}

void DisplayWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
	Q_UNUSED(event);
	emit doubleClicked();
}

QString	DisplayWidget::tooltipText() const
{
    if (m_projects == 0) return QString();

	if (m_projects->connectionError()) {
		return tr("Cannot connect to server. Please set the options.");
	}

	int numProjects = m_projects->size();
	if (numProjects == 0) {
		return tr("no projects");
	}

	QString rc;
    rc += QString::fromAscii("<table width=\"300\">\n");
	for (int i = 0 ; i < numProjects; i++) {
        QString color = m_projects->passed(i) ? QString::fromAscii("green") : QString::fromAscii("red");

        rc += QString::fromAscii("<tr>");
        rc += QString::fromAscii("<td><font color=\"%1\">%2</font></td>").arg(color).arg(m_projects->name(i));
        rc += QString::fromAscii("<td><font color=\"%1\">%2</font></td>").arg(color).arg(m_projects->date(i));
        rc += QString::fromAscii("</tr>\n");
	}
    rc += QString::fromAscii("</table>\n");

	return rc;
}

