#ifndef OPENRGBFANSYNCPLUGIN_H
#define OPENRGBFANSYNCPLUGIN_H

#include "OpenRGBPluginInterface.h"
#include "ResourceManager.h"

#include <QObject>
#include <QString>
#include <QtPlugin>
#include <QWidget>

class OpenRGBFanSyncPlugin : public QObject, public OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID)
    Q_INTERFACES(OpenRGBPluginInterface)

private:
    QWidget* widget;

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

};

#endif // OPENRGBFANSYNCPLUGIN_H
