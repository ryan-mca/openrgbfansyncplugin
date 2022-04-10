#ifndef HARDWAREMONITOR_H
#define HARDWAREMONITOR_H

#include <QString>
#include <QTimer>
#include <unordered_set>

#include <lhwm-cpp-wrapper.h>

class HardwareMonitor : public QObject
{
Q_OBJECT
private:
    QTimer *updateTimer;

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
    /// A list of enabled sensors and their values [identifier, value]
    /// </summary>
    std::map<std::string, double> SensorValue;

    ///
    /// \brief updated the value for all key present in the SensorValue map
    ///
    void updateValues();

    ///
    /// \brief startAutoUpdate
    /// \param intervalMs optional time interal in milliseconds (default: 1000 ms)
    ///
    void startAutoUpdate(int intervalMs = 1000);

    ///
    /// \brief stopAutoUpdate stops the auto update timer
    ///
    void stopAutoUpdate();

    HardwareMonitor();
};

#endif // HARDWAREMONITOR_H
