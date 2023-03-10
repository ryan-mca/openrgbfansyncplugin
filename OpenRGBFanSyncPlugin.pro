QT +=                  \
    gui                \
    widgets            \
    core               \
    printsupport       \

win32:CONFIG += QTPLUGIN

TEMPLATE = lib
DEFINES += ORGBFANSYNCPLUGIN_LIBRARY

win32:CONFIG += c++17

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
PLUGIN_VERSION     = 0.1

#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID   = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse HEAD)
GIT_COMMIT_DATE = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ show -s --format=%ci HEAD)
GIT_BRANCH      = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse --abbrev-ref HEAD)

#-----------------------------------------------------------------------------------------------#
# Inject vars in defines                                                                        #
#-----------------------------------------------------------------------------------------------#
DEFINES +=                                                                                      \
    VERSION_STRING=\\"\"\"$$PLUGIN_VERSION\\"\"\"                                               \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                                                  \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                                                 \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                                             \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"                                                       \
    LATEST_BUILD_URL=\\"\"\"$$LATEST_BUILD_URL\\"\"\"                                           \

#-------------------------------------------------------------------#
# Includes                                                          #
#-------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    OpenRGB/                                                                                    \
    OpenRGB/i2c_smbus                                                                           \
    OpenRGB/RGBController                                                                       \
    OpenRGB/dependencies/json                                                                   \
    OpenRGB/net_port                                                                            \
    dependencies/exprtk                                                                         \
    dependencies/qcustomplot                                                                    \

HEADERS +=                                                                                      \
    FanCurvePlotWidget.h \
    FanSyncPage.h \
    HardwareMonitor.h \
    OpenRGB/OpenRGBPluginInterface.h                                                            \
    OpenRGB/ResourceManager.h                                                                   \
    OpenRGB/dependencies/json/json.hpp                                                          \
    OpenRGB/qt/TabLabel.h                                                                       \
    Settings.h                                                                                  \
    dependencies/exprtk/exprtk.hpp                                                              \
    dependencies/qcustomplot/qcustomplot.h                                                      \
    OpenRGBFanSyncPlugin.h                                                                      \
    FanSyncWidget.h                                                                             \

SOURCES +=                                                                                      \
    FanCurvePlotWidget.cpp \
    FanSyncPage.cpp \
    FanSyncWidget.cpp                                                                           \
    HardwareMonitor.cpp \
    OpenRGBFanSyncPlugin.cpp                                                                    \
    OpenRGB/qt/TabLabel.cpp                                                                     \
    Settings.cpp \
    dependencies/qcustomplot/qcustomplot.cpp                                                    \

FORMS +=                                                                                        \
    OpenRGB/qt/TabLabel.ui                                                                      \

#-------------------------------------------------------------------#
# Windows GitLab CI Configuration                                   #
#-------------------------------------------------------------------#
win32:CONFIG(debug, debug|release) {
    win32:DESTDIR = debug
}

win32:CONFIG(release, debug|release) {
    win32:DESTDIR = release
}

win32:OBJECTS_DIR = _intermediate_$$DESTDIR/.obj
win32:MOC_DIR     = _intermediate_$$DESTDIR/.moc
win32:RCC_DIR     = _intermediate_$$DESTDIR/.qrc
win32:UI_DIR      = _intermediate_$$DESTDIR/.ui

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                             \
        -lws2_32                                                        \
        -lole32                                                         \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                             \
        -lws2_32                                                        \
        -lole32                                                         \
}

win32:DEFINES +=                                                        \
    _MBCS                                                               \
    WIN32                                                               \
    _CRT_SECURE_NO_WARNINGS                                             \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                     \
    WIN32_LEAN_AND_MEAN                                                 \

win32:{
    INCLUDEPATH += dependencies/lhwm-cpp-wrapper
    DEPENDPATH += dependencies/lhwm-cpp-wrapper
    HEADERS += dependencies/lhwm-cpp-wrapper/lhwm-cpp-wrapper.h
    QMAKE_CXXFLAGS += /bigobj
}

win32:CONFIG(debug, debug|release):contains(QMAKE_TARGET.arch, x86_64) {
    LIBS += -L$$PWD/dependencies/lhwm-cpp-wrapper/x64/Debug -llhwm-cpp-wrapper
}

win32:CONFIG(release, debug|release):contains(QMAKE_TARGET.arch, x86_64) {
    LIBS += -L$$PWD/dependencies/lhwm-cpp-wrapper/x64/Release -llhwm-cpp-wrapper
}

#-----------------------------------------------------------------------#
# Linux-specific Configuration                                          #
#-----------------------------------------------------------------------#
unix:!macx {

    INCLUDEPATH +=                                                      \
        dependencies/libsensors-cpp                                     \

    HEADERS +=                                                          \
        dependencies/libsensors-cpp/sensors.h                           \
        dependencies/libsensors-cpp/error.h                             \

    SOURCES +=                                                          \
        dependencies/libsensors-cpp/sensors.cpp                         \
        dependencies/libsensors-cpp/error.cpp                           \

    LIBS += -lsensors -lstdc++fs
    QMAKE_CXXFLAGS += -std=c++17
}



RESOURCES += \
    resources.qrc
