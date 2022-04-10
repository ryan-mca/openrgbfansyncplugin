#ifndef OPENRGBFANSYNCPLUGIN_H
#define OPENRGBFANSYNCPLUGIN_H

#include "OpenRGBPluginInterface.h"
#include "ResourceManager.h"

#include <QObject>
#include <QString>
#include <QtPlugin>
#include <QWidget>
#include <QTabWidget>
#include <QFile>
#include <QThread>

#include "HardwareMonitor.h"
#include "FanSyncWidget.h"

class OpenRGBFanSyncPlugin : public QObject, public OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID)
    Q_INTERFACES(OpenRGBPluginInterface)

private:
    HardwareMonitor* MainHardwareMonitor;
    FanSyncWidget* MainWidget;
    QThread workerThread;

public:
    OpenRGBFanSyncPlugin();
    ~OpenRGBFanSyncPlugin();

    OpenRGBPluginInfo   GetPluginInfo() override;
    unsigned int        GetPluginAPIVersion() override;

    void                Load(bool dark_theme, ResourceManager* resource_manager_ptr) override;
    QWidget*            GetWidget() override;
    QMenu*              GetTrayMenu() override;
    void                Unload() override;


    static bool             DarkTheme;
    static ResourceManager* RMPointer;

signals:
    void autoUpdateHardwareMonitor(const int &);

};

#endif // OPENRGBFANSYNCPLUGIN_H
