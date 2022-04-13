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
#ifdef _WIN32
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
#elif __linux__
    chips = sensors::get_detected_chips();

    for (int ChipID = 0; ChipID < (int)chips.size(); ChipID++)
    {
        std::string HardwareName = chips[ChipID].name();

        sensors::chip_name chip = chips[ChipID];
        for (int ChipFeatureID = 0; ChipFeatureID < (int)chip.features().size(); ChipFeatureID++)
        {
            sensors::feature FTR = chip.features()[ChipFeatureID];
            std::string BaseFeatureName = (std::string)FTR.name();

            for(int SubFeatureID = 0; SubFeatureID < (int)FTR.subfeatures().size(); SubFeatureID++)
            {
                sensors::subfeature SubFTR = FTR.subfeatures()[SubFeatureID];
                std::string FTRName = (BaseFeatureName + std::string(SubFTR.name()).c_str());
                std::string Identifier = "/" + std::to_string(ChipID) + "/" + std::to_string(ChipFeatureID) + "/" + std::to_string(SubFeatureID);
                if(SubFTR.type() == sensors::subfeature_type::input && SubFTR.readable())
                {
                    sensorList[Identifier] = HardwareName + " - " + BaseFeatureName + " - " + FTRName;
                }
                else if(FTR.type() == sensors::feature_type::fan && SubFTR.writable())
                {
                    controlHardwareList[Identifier] = HardwareName + " - " + BaseFeatureName + " - " + FTRName;
                    sensorValue[Identifier] = 0;
                }
            }
        }
    }

#endif
}

void HardwareMonitor::updateSensors()
{
    for(auto& sensor : sensorValue)
    {
        if (sensorList.find(sensor.first) == sensorList.end() && controlHardwareList.find(sensor.first) == controlHardwareList.end())
        {
            continue;
        }
#ifdef _WIN32
        sensor.second = LHWM::GetSensorValue(sensor.first);
#elif __linux__
        QStringList idArray = QString::fromStdString(sensor.first).split('/', Qt::SkipEmptyParts);
        if (idArray.count() != 3)
        {
            continue;
        }
        try {
            sensor.second = (float)chips[idArray[0].toInt()].features()[idArray[1].toInt()].subfeatures()[idArray[2].toInt()].read();
        }
        catch (...) {}
#endif
    }

    emit sensorsUpdated();
}

void HardwareMonitor::startAutoUpdate(int intervalMs)
{
    if(!updateTimer)
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
#ifdef _WIN32
        LHWM::SetControlValue(identifier, value);
#elif __linux__
        QStringList idArray = QString::fromStdString(identifier).split('/', Qt::SkipEmptyParts);
        if (idArray.count() != 3)
        {
            return;
        }
        try {
            chips[idArray[0].toInt()].features()[idArray[0].toInt()].subfeatures()[idArray[0].toInt()].write(value);
        }
        catch (...) {}
#endif
    }
}
