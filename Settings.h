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
// File adapted from: https://gitlab.com/OpenRGBDevelopers/OpenRGBHardwareSyncPlugin/-/blob/master/MeasureSettings.h
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include "json.hpp"
#include <fstream>
#include <iostream>
#include <QFile>
#include <QString>
#include <QDir>

using json = nlohmann::json;

class Settings
{
public:
    ///
    /// \brief Save stores json to the settings file
    /// \param key to store the json under, can be "" to write overwrite the whole file
    /// \param value value to store
    /// \return true if successful, false otherwise
    ///
    static bool Save(std::string key, json value);

    ///
    /// \brief Loads json from the settings file
    /// \param key property key to read, "" to return the whole document
    /// \return result json
    ///
    static json Load(std::string key = "");

private:
    static bool CreateSettingsDirectory();

    static std::string folder_separator();
    static std::string SettingsFolder();

    static bool create_dir(std::string);
    static json load_json_file(std::string);
    static bool write_file(std::string, json);

};

#endif // SETTINGS_H
