#ifndef CRUISECONTROLPLUGIN_H
#define CRUISECONTROLPLUGIN_H

#include "projects.h"
#include "cruisecontrolsetingspage.h"
#include <extensionsystem/iplugin.h>


#include <QSharedPointer>
#include <QStringList>
#include <QTimer>

namespace CruiseControl {
namespace Internal {

struct CruiseControlSettings;
class DataFetcher;

class CruiseControlPlugin  : public ExtensionSystem::IPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.CruiseControlPlugin")
public:
	CruiseControlPlugin();
	~CruiseControlPlugin();

	bool initialize(const QStringList &arguments, QString *error_message);
	void extensionsInitialized();
	void shutdown();


private slots:

	void	refresh();
	void	readFinished(bool error);
	void	openResults();

private:


	QSharedPointer<CruiseControlSettings>	m_settings;
	Projects*								m_projects;
	DataFetcher*							m_fetcher;
	QTimer*									m_timer;
	bool									m_reading;
};

} // namespace Internal
} // namespace CruiseControl

#endif // CruiseControlPLUGIN_H
