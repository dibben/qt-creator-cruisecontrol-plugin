TARGET = CruiseControl

DEFINES += CRUISECONTROL_LIBRARY

TEMPLATE = lib
QT += xml network

PROVIDER = CreatorPlugins

SOURCES += cruisecontrolplugin.cpp \
	cruisecontrolsetingspage.cpp \
	displaywidget.cpp \
	projects.cpp \
	datafetcher.cpp

HEADERS += cruisecontrolplugin.h \
	cruisecontrolsetingspage.h \
	cruisecontrolconstants.h \
	displaywidget.h \
	projects.h \
	datafetcher.h

FORMS += cruisecontrolsetingspage.ui

RESOURCES += cruisecontrol.qrc

OTHER_FILES += \
    CruiseControl.json.in

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/dibben/Develop/QtCreator/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/dibben/Develop/QtCreator/qt-creator-build-5.3

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\Nokia\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/Nokia/qtcreator" or "~/.local/share/Nokia/qtcreator" on Linux
##    "~/Library/Application Support/Nokia/Qt Creator" on Mac
#USE_USER_DESTDIR = yes

LIBS += -L$$IDE_PLUGIN_PATH/QtProject

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
