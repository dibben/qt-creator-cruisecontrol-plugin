qt-creator-cruisecontrol-plugin
===============================

A plugin for QtCreator which monitors a Cruise Control server

Tested with Qt-Creator 2.8 and Qt-Creator 3.0.


Compiling and Installation
============================

Just generate build directory and relevant makefiles with QMake. Customize
input variables QTC_SOURCE and QTC_BUILD for your particular setup.

	cd <path-to-cruisecontrol-sources>
	mkdir build
	cd build
	qmake QTC_SOURCE=<path-to-qt-creator-src> QTC_BUILD=<path-to-qt-creator-build-dir> ../
	make

This will build and copy the plugin to the qt-creator plugin directory.

