#include "OpenRGBFanSyncPlugin.h"
#include "MainWindow.h"

bool OpenRGBFanSyncPlugin::DarkTheme = false;
ResourceManager* OpenRGBFanSyncPlugin::RMPointer = nullptr;

OpenRGBPluginInfo OpenRGBFanSyncPlugin::GetPluginInfo()
{
    printf("[OpenRGBFanSyncPlugin] Loading plugin info.\n");

    OpenRGBPluginInfo info;
    info.Name         = "Hardware Fan Sync";
    info.Description  = "Set Fan curves based on hardware sensors";
    info.Version  = VERSION_STRING;
    info.Commit  = GIT_COMMIT_ID;
    info.URL  = "https://gitlab.com/ShadyNawara/openrgbfansyncplugin";
    info.Icon.load(":/OpenRGBFanSyncPlugin.png");

    info.Location     =  OPENRGB_PLUGIN_LOCATION_TOP;
    info.Label        =  "Hardware Fan Sync";
    info.TabIconString        =  "Hardware Fan Sync";
    info.TabIcon.load(":/OpenRGBFanSyncPlugin.png");

    return info;
}

unsigned int OpenRGBFanSyncPlugin::GetPluginAPIVersion()
{
    printf("[OpenRGBFanSyncPlugin] Loading plugin API version.\n");

    return OPENRGB_PLUGIN_API_VERSION;
}

void OpenRGBFanSyncPlugin::Load(bool dark_theme, ResourceManager* resource_manager_ptr)
{
    printf("[OpenRGBFanSyncPlugin] Loading plugin.\n");

    RMPointer                = resource_manager_ptr;
    DarkTheme                = dark_theme;
}


QWidget* OpenRGBFanSyncPlugin::GetWidget()
{
    printf("[OpenRGBFanSyncPlugin] Creating widget.\n");

    return widget;
}

QMenu* OpenRGBFanSyncPlugin::GetTrayMenu()
{
    printf("[OpenRGBFanSyncPlugin] Creating tray menu.\n");

    QMenu* menu = new QMenu("Sample plugin");

    return menu;
}

void OpenRGBFanSyncPlugin::Unload()
{
    printf("[OpenRGBFanSyncPlugin] Time to call some cleaning stuff.\n");
}

OpenRGBFanSyncPlugin::OpenRGBFanSyncPlugin()
{
    widget = new MainWindow(nullptr);
}

OpenRGBFanSyncPlugin::~OpenRGBFanSyncPlugin()
{
     delete widget;
}

