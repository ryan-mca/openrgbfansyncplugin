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
                ControlHardwareList[std::get<2>(hardwareSensor)] = HardwareName + " - " + std::get<0>(hardwareSensor);
            }
            else
            {
                SensorList[std::get<2>(hardwareSensor)] = HardwareName + " - " + std::get<0>(hardwareSensor);
            }
        }
    }
}

void HardwareMonitor::updateValues()
{
    for(auto& sensor : SensorValue)
    {
        sensor.second = LHWM::GetSensorValue(sensor.first);
    }
}

void HardwareMonitor::startAutoUpdate(int intervalMs)
{
    if(updateTimer)
    {
        updateTimer = new QTimer();
        QObject::connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&HardwareMonitor::updateValues));
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

