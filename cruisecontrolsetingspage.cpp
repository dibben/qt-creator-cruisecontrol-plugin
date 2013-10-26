#include "cruisecontrolsetingspage.h"
#include "cruisecontrolconstants.h"
#include <coreplugin/icore.h>

#include <QSettings>
#include <QString>

#include "ui_cruisecontrolsetingspage.h"

namespace CruiseControl {
namespace Internal {


CruiseControlSettings::CruiseControlSettings()
{
	refresh = 360;
}

void CruiseControlSettings::toSettings(QSettings *s) const
{
	s->beginGroup(QLatin1String(Constants::CRUISECONTROL_SETTINGSGROUP));
	s->setValue(QLatin1String(Constants::CRUISECONTROL_URL), url);
	s->setValue(QLatin1String(Constants::CRUISECONTROL_IGNORED), ignore);
	s->setValue(QLatin1String(Constants::CRUISECONTROL_REFRESH), refresh);
	s->endGroup();
}

void CruiseControlSettings::fromSettings(QSettings *s)
{
	s->beginGroup(QLatin1String(Constants::CRUISECONTROL_SETTINGSGROUP));
	url= s->value(QLatin1String(Constants::CRUISECONTROL_URL), QLatin1String("")).toString();
	ignore = s->value(QLatin1String(Constants::CRUISECONTROL_IGNORED), QLatin1String("")).toString();
	refresh= s->value(QLatin1String(Constants::CRUISECONTROL_REFRESH), 360).toInt();
	s->endGroup();
}


bool CruiseControlSettings::equals(const CruiseControlSettings &rhs) const
{
	return (url == rhs.url && ignore == rhs.ignore && refresh == rhs.refresh);
}

// ------------------ CruiseControlSetingsWidget

CruiseControlSettingsWidget::CruiseControlSettingsWidget(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::CruiseControlSetingsWidget)
{
    ui->setupUi(this);
}

CruiseControlSettingsWidget::~CruiseControlSettingsWidget()
{
    delete ui;
}



CruiseControlSettings CruiseControlSettingsWidget::settings() const
{
	CruiseControlSettings rc;
	rc.url = ui->urlEdit->text();
	rc.ignore = ui->ignoreEdit->text();
	rc.refresh = ui->refreshSpinBox->value();
	return rc;
}


void CruiseControlSettingsWidget::setSettings(const CruiseControlSettings &s)
{
	ui->urlEdit->setText(s.url );
	ui->ignoreEdit->setText(s.ignore);
	ui->refreshSpinBox->setValue(s.refresh);
}


// --------------- CruiseControlSetingsPage

CruiseControlSettingsPage::CruiseControlSettingsPage(QSharedPointer<CruiseControlSettings> &settings,
										 QObject *parent) :
	Core::IOptionsPage(parent),
	m_settings(settings)
{
    setId(Constants::CRUISECONTROL_SETTINGS_ID);
    setDisplayName( QCoreApplication::translate("CruiseControl", Constants::CRUISECONTROL_SETTINGS_ID));
    setCategory( Constants::CRUISECONTROL_SETTINGS_CATEGORY);
    setDisplayCategory( QCoreApplication::translate("CruiseControl", Constants::CRUISECONTROL_SETTINGS_CATEGORY));
    setCategoryIcon(QLatin1String(":/cruisecontrol/resources/cruise.ico"));
}

CruiseControlSettingsPage::~CruiseControlSettingsPage()
{
}


QWidget *CruiseControlSettingsPage::createPage(QWidget *parent)
{

	m_widget = new CruiseControlSettingsWidget(parent);
	m_widget->setSettings(*m_settings);
	return m_widget;
}

void CruiseControlSettingsPage::apply()
{
	if (m_widget) {
		const CruiseControlSettings newSettings = m_widget->settings();
		if (newSettings != *m_settings) {
			*m_settings = newSettings;
			m_settings->toSettings(Core::ICore::instance()->settings());
			emit settingsUpdated();
		}
	}
}


} // namespace Internal
} // namespace CruiseControl
