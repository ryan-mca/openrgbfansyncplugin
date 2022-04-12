//
// File adapted from: https://gitlab.com/OpenRGBDevelopers/OpenRGBHardwareSyncPlugin/-/blob/master/MeasureSettings.cpp
//

#include "Settings.h"
#include "OpenRGBFanSyncPlugin.h"

bool Settings::Save(std::string key, json j)
{
    printf("[OpenRGBFanSyncPlugin] Saving file.\n");

    if(!CreateSettingsDirectory())
    {
        printf("[OpenRGBFanSyncPlugin] Cannot create settings directory.\n");
        return false;
    }

    if (key == "")
    {
        return write_file(SettingsFolder() + folder_separator() + "FanSyncSettings.json", j);
    }
    else
    {
        json existing = Settings::Load();
        existing[key] = j;
        return write_file(SettingsFolder() + folder_separator() + "FanSyncSettings.json", existing);
    }
}

json Settings::Load(std::string key)
{
    printf("[OpenRGBFanSyncPlugin] Loading file.\n");

    json Settings;

    if(!CreateSettingsDirectory())
    {
        return Settings;
    }

    Settings = load_json_file(SettingsFolder() + folder_separator() + "FanSyncSettings.json");

    if (key == "")
    {
        return Settings;
    }
    else
    {
        return Settings[key];
    }
}


std::string Settings::folder_separator()
{
#if defined(WIN32) || defined(_WIN32)
    return "\\";
#else
    return "/";
#endif
}

bool Settings::write_file(std::string file_name, json j)
{
    std::ofstream file(file_name, std::ios::out | std::ios::binary);

    if(file)
    {
        try
        {
            file << j.dump(4);
            file.close();
        }
        catch(const std::exception& e)
        {
            printf("[OpenRGBFanSyncPlugin] Cannot write file: %s\n", e.what());
            return false;
        }
    }

    return true;
}

json Settings::load_json_file(std::string file_name)
{
    json j;

    std::ifstream file(file_name);

    if(file)
    {
        try
        {
            file >> j;
            file.close();
        }
        catch(const std::exception& e)
        {
             printf("[OpenRGBFanSyncPlugin] Cannot read file: %s\n", e.what());
        }
    }

    return j;
}

bool Settings::create_dir(std::string directory)
{
    QDir dir(QString::fromStdString(directory));

    if(dir.exists())
    {
        return true;
    }

    return QDir().mkpath(dir.path());
}


std::string Settings::SettingsFolder()
{
    return OpenRGBFanSyncPlugin::RMPointer->GetConfigurationDirectory() + "plugins" + folder_separator() + "settings";
}

bool Settings::CreateSettingsDirectory()
{
     return create_dir(SettingsFolder());
}
