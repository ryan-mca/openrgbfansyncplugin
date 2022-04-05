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
                SensorEnabled[std::get<2>(hardwareSensor)] = true; // control sensors are enabled by default
            }
            else
            {
                SensorList[std::get<2>(hardwareSensor)] = HardwareName + " - " + std::get<0>(hardwareSensor);
                SensorEnabled[std::get<2>(hardwareSensor)] = false; // control sensors are disabled by default
            }

            SensorValues[std::get<2>(hardwareSensor)] = 0;
        }
    }


}
