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

#include "HardwareMonitor.h"

HardwareMonitor::HardwareMonitor()
{
    // hardware sensor map from the librehardwaremonitor wrapper
    // [hardware name, [sensor name, sensor type, sensor identifier]]
    std::map<std::string, std::vector<std::tuple<std::string, std::string, std::string>>>
                hs_map = LHWM::GetHardwareSensorMap();

    for (const auto& [HardwareName, HardwareSensors] : hs_map)
    {
        for (const auto& hardwareSensor : HardwareSensors)
        {
            if(std::get<1>(hardwareSensor) == "Control")
            {
                controlHardwareList[std::get<2>(hardwareSensor)] = HardwareName + " - " + std::get<0>(hardwareSensor);
                sensorValue[std::get<2>(hardwareSensor)] = 0;
            }
            else
            {
                sensorList[std::get<2>(hardwareSensor)] = HardwareName + " - " + std::get<1>(hardwareSensor) + " - " + std::get<0>(hardwareSensor);
                sensorListHierarchy[HardwareName][std::get<1>(hardwareSensor)][std::get<0>(hardwareSensor)] = std::get<2>(hardwareSensor);
            }
        }
    }
}

void HardwareMonitor::updateSensors()
{
    for(auto& sensor : sensorValue)
    {
        if (!sensorList.count(sensor.first) && !controlHardwareList.count(sensor.first))
        {
            continue;
        }

        sensor.second = LHWM::GetSensorValue(sensor.first);
    }

    emit sensorsUpdated();
}

void HardwareMonitor::startAutoUpdate(int intervalMs)
{
    if(updateTimer)
    {
        updateTimer = new QTimer();
        QObject::connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&HardwareMonitor::updateSensors));
        updateTimer->start(intervalMs);
    }
}

void HardwareMonitor::stopAutoUpdate()
{
    if(updateTimer)
    {
        updateTimer->stop();
        delete updateTimer;
    }
}

void HardwareMonitor::setControlValue(std::string identifier, float value)
{
    if (sensorValue[identifier] != value || value == 0)
    {
        LHWM::SetControlValue(identifier, value);
    }
}
