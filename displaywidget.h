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
