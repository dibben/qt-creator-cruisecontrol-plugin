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
