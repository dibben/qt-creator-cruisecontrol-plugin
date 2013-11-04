TEMPLATE = lib

QT += xml network

TARGET = CruiseControl
PROVIDER = CreatorPlugins
DEFINES += CRUISECONTROL_LIBRARY


IDE_SOURCE_TREE = /home/dibben/Develop/qt-creator
IDE_BUILD_TREE = /home/dibben/Develop/qt-creator-build-5.2

DESTDIR = $$IDE_BUILD_TREE/lib/qtcreator/plugins/$$PROVIDER
LIBS += -L$$IDE_BUILD_TREE/lib/qtcreator/plugins/QtProject

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)


DEFINES -= QT_NO_CAST_FROM_ASCII


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

OTHER_FILES += CruiseControl.pluginspec
