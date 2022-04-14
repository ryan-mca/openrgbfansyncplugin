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

#ifndef HARDWAREMONITOR_H
#define HARDWAREMONITOR_H

#include <QString>
#include <QTimer>
#include <unordered_set>

#ifdef _WIN32
#include <lhwm-cpp-wrapper.h>
#elif __linux__
#include <sensors.h>
#endif


class HardwareMonitor : public QObject
{
Q_OBJECT
private:
    QTimer *updateTimer {};

#ifdef __linux__
std::vector<sensors::chip_name> chips;
#endif


public:
    /// <summary>
    /// A list of control devices detect [identifier, name]
    /// </summary>
    std::map<std::string, std::string> controlHardwareList;

    /// <summary>
    /// A list of sensors detected [identifier, name]
    /// </summary>
    std::map<std::string, std::string> sensorList;

    /// <summary>
    /// A list of sensors detected [identifier, name]
    /// </summary>
    std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> sensorListHierarchy;

    /// <summary>
    /// A list of enabled sensors and their values [identifier, value]
    /// </summary>
    std::map<std::string, float> sensorValue;

    ///
    /// \brief updated the value for all key present in the SensorValue map
    ///
    void updateSensors();

    ///
    /// \brief startAutoUpdate
    /// \param intervalMs optional time interal in milliseconds (default: 1000 ms)
    ///
    void startAutoUpdate(int intervalMs = 1000);

    ///
    /// \brief stopAutoUpdate stops the auto update timer
    ///
    void stopAutoUpdate();

    ///
    /// \brief setControlValue sets the value of the control device (i.e fan speed)
    /// \param identifier control identifier
    /// \param value new control value
    ///
    void setControlValue(std::string identifier, float value);

    ///
    /// \brief HardwareMonitor responsible for communicating with the lhwm wrapper
    ///
    HardwareMonitor();

signals:
    ///
    /// \brief sensorsUpdated signal is emitted whenever the sensors are updated (through updateSensors())
    ///
    void sensorsUpdated();

};

#endif // HARDWAREMONITOR_H
