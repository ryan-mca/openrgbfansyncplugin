// OpenRGBFanSyncPlugin (https://gitlab.com/ShadyNawara/openrgbfansyncplugin)
// Copyright (C) 2022 Shady Nawara
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
