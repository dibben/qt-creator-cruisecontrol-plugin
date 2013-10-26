#ifndef CRUISECONTROLCONSTANTS_H
#define CRUISECONTROLCONSTANTS_H


namespace CruiseControl {
namespace Constants {


// QSettings keys for use by the "New Class" wizards.
const char * const CRUISECONTROL_SETTINGSGROUP = "CruiseControl";
const char * const CRUISECONTROL_URL = "URL";
const char * const CRUISECONTROL_IGNORED = "Ignored";
const char * const CRUISECONTROL_REFRESH = "RefreshInterval";

const char * const CRUISECONTROL_SETTINGS_ID = QT_TRANSLATE_NOOP("CruiseControl", "Cruise Control Options");
const char * const CRUISECONTROL_SETTINGS_CATEGORY = QT_TRANSLATE_NOOP("CruiseControl", "Cruise Control");


} // namespace Constants
} // namespace CppHelper

#endif // CRUISECONTROLCONSTANTS_H
