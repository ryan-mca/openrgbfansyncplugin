#include "OpenRGBFanSyncPlugin.h"

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

    bool can_load = QFile(QCoreApplication::applicationDirPath()+"\\lhwm-wrapper.dll").exists();

    if (can_load)
    {
        hardwareMonitor = new HardwareMonitor();
        mainWidget = new FanSyncWidget(hardwareMonitor);

        hardwareMonitor->moveToThread(&workerThread);
        mainWidget->moveToThread(&workerThread);

        connect(&workerThread, &QThread::finished, hardwareMonitor, &QObject::deleteLater);
        connect(&workerThread, &QThread::finished, mainWidget, &QObject::deleteLater);

        connect(this, &OpenRGBFanSyncPlugin::autoUpdateHardwareMonitor, hardwareMonitor, &HardwareMonitor::startAutoUpdate);
        workerThread.start();

        emit autoUpdateHardwareMonitor(1000);

        return mainWidget;
    }
    else
    {
        QLabel* label = new QLabel(
                        "<h1>Cannot load the plugin.</h1>"
                        "<p>Make sure you downloaded <a href=\"https://gitlab.com/ShadyNawara/openrgbfansyncplugin/-/raw/master/dependencies/lhwm-cpp-wrapper/x64/Release/lhwm-wrapper.dll\">lhwm-wrapper.dll</a></p>"
                        "<p>Place this DLL inside  <b>" + QCoreApplication::applicationDirPath() +  "</b> and restart OpenRGB.</p>"
         );

        label->setTextFormat(Qt::RichText);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setOpenExternalLinks(true);

        return label;
    }
}

QMenu* OpenRGBFanSyncPlugin::GetTrayMenu()
{
    printf("[OpenRGBFanSyncPlugin] Creating tray menu.\n");

    QMenu* menu = new QMenu("Hardware Fan Sync");

    return menu;
}

void OpenRGBFanSyncPlugin::Unload()
{
    workerThread.quit();
    workerThread.wait();
}

OpenRGBFanSyncPlugin::OpenRGBFanSyncPlugin()
{

}

OpenRGBFanSyncPlugin::~OpenRGBFanSyncPlugin()
{

}

