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

#include "cruisecontrolplugin.h"
#include "datafetcher.h"
#include "cruisecontrolsetingspage.h"
#include "displaywidget.h"

#include <utils/synchronousprocess.h>

#include <vcsbase/vcsbaseoutputwindow.h>

#include "projectexplorer/projectexplorer.h"
#include "projectexplorer/project.h"

#include <cppeditor/cppeditorconstants.h>

#include <cpptools/cpptoolsconstants.h>

#include <coreplugin/coreconstants.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/icore.h>
#include <coreplugin/modemanager.h>


#include <QtPlugin>
#include <QAction>
#include <QKeySequence>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QDesktopServices>


using namespace CruiseControl;
using namespace CruiseControl::Internal;

CruiseControlPlugin::CruiseControlPlugin() :
    m_settings(new CruiseControlSettings),
    m_projects(new Projects),
    m_fetcher(new DataFetcher(20, this)),
    m_timer(new QTimer(this))
{

    connect(m_fetcher, SIGNAL(finished(bool)), this, SLOT(readFinished(bool)));
    connect(m_fetcher, SIGNAL(projectItemReady(QString,QString,bool,QString)),
            m_projects, SLOT(add(QString,QString,bool,QString)));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(refresh()));

    m_reading = false;
}

CruiseControlPlugin::~CruiseControlPlugin()
{

}

bool CruiseControlPlugin::initialize(const QStringList &arguments, QString *error_message)
{
    Q_UNUSED(arguments);
    Q_UNUSED(error_message);

    CruiseControlSettingsPage* page = new CruiseControlSettingsPage(m_settings);
    addAutoReleasedObject(page);
    connect(page, SIGNAL(settingsUpdated()), this, SLOT(refresh()));

    DisplayWidget* w = new DisplayWidget(m_projects);
    Core::ModeManager::addWidget(w);
    connect(w, SIGNAL(refreshRequested()), this, SLOT(refresh()));
    connect(w, SIGNAL(doubleClicked()), this, SLOT(openResults()));

    return true;
}

void  CruiseControlPlugin::refresh()
{
    if (m_reading) return;
    m_reading = true;
    m_timer->stop();
    m_projects->clear();
    m_projects->setIgnored(m_settings->ignore);
    m_fetcher->fetch(QUrl(m_settings->url));
}

void  CruiseControlPlugin::readFinished(bool error)
{
    m_reading = false;
    m_projects->setConnectionError(error);
    int seconds = m_settings->refresh;
    if (seconds < 60) seconds = 60;
    m_timer->start(seconds * 1000);
}

void CruiseControlPlugin::openResults()
{
    QString url = m_projects->resultsUrl();
    if (url.isEmpty()) return;

    QDesktopServices::openUrl(url);
}

void CruiseControlPlugin::extensionsInitialized()
{
    m_settings->fromSettings(Core::ICore::instance()->settings());

    refresh();
}

void CruiseControlPlugin::shutdown()
{
    m_timer->stop();
}


Q_EXPORT_PLUGIN(CruiseControlPlugin)


