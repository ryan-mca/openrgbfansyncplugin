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
