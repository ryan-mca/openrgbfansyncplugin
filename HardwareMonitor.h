#ifndef HARDWAREMONITOR_H
#define HARDWAREMONITOR_H

#include <QString>

#include <lhwm-cpp-wrapper.h>

class HardwareMonitor
{
public:
    /// <summary>
    /// A list of control devices detect [identifier, name]
    /// </summary>
    std::map<std::string, std::string> ControlHardwareList;

    /// <summary>
    /// A list of sensors detected [identifier, name]
    /// </summary>
    std::map<std::string, std::string> SensorList;

    /// <summary>
    /// A list of all sensors and their values [identifier, value]
    /// </summary>
    std::map<std::string, double> SensorValues;

    /// <summary>
    /// A list of which sensors are in use
    /// only enabled sensors would be updated
    /// </summary>
    std::map<std::string, bool> SensorEnabled;

    HardwareMonitor();
};

#endif // HARDWAREMONITOR_H
