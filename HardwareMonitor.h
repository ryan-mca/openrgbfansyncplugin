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
    std::map<std::string, std::string> controlHardwareList;

    /// <summary>
    /// A list of sensors detected [identifier, name]
    /// </summary>
    std::map<std::string, std::string> sensorList;

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
