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

#ifndef CRUISECONTROLSETINGSPAGE_H
#define CRUISECONTROLSETINGSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>
#include <QWidget>
#include <QSettings>
#include <QPointer>


namespace CruiseControl {
namespace Internal {

namespace Ui {
class CruiseControlSetingsWidget;
}


struct CruiseControlSettings {
    CruiseControlSettings();

    QString url;
    QString ignore;
    int		refresh;

    void toSettings(QSettings *) const;
    void fromSettings(QSettings *);

    bool equals(const CruiseControlSettings &rhs) const;
};

inline bool operator==(const CruiseControlSettings &s1, const CruiseControlSettings &s2) { return s1.equals(s2); }
inline bool operator!=(const CruiseControlSettings &s1, const CruiseControlSettings &s2) { return !s1.equals(s2); }


class CruiseControlSettingsWidget : public QWidget {
    Q_OBJECT
public:
    explicit CruiseControlSettingsWidget(QWidget *parent = 0);
    ~CruiseControlSettingsWidget();

    CruiseControlSettings settings() const;
    void setSettings(const CruiseControlSettings &s);

private:
    Ui::CruiseControlSetingsWidget *ui;
};


class CruiseControlSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT
    Q_DISABLE_COPY(CruiseControlSettingsPage)
public:
    explicit CruiseControlSettingsPage(QSharedPointer<CruiseControlSettings> &settings,
                                       QObject *parent = 0);
    virtual ~CruiseControlSettingsPage();


    virtual QWidget *createPage(QWidget *parent);
    virtual QWidget *widget();
    virtual void apply();
    virtual void finish() { }

signals:

    void	settingsUpdated();

private:
    const QSharedPointer<CruiseControlSettings> m_settings;
    QPointer<CruiseControlSettingsWidget> m_widget;
};

} // namespace Internal
} // namespace CruiseControl
#endif // CruiseControlSetingsWidget_H
