#ifndef FANSYNCPAGE_H
#define FANSYNCPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>

#include <exprtk.hpp>

#include "HardwareMonitor.h"
#include "FanCurvePlotWidget.h"

class FanSyncPage : public QWidget
{
    Q_OBJECT
public:
    FanSyncPage(std::string controlIdentifier, HardwareMonitor *hardwareMonitor, QWidget *parent = nullptr);

private:
    std::string controlIdentifier;
    HardwareMonitor *hardwareMonitor;
    QComboBox *sensorDropdown;
    QPlainTextEdit *measureFunctionText;
    QLabel *selectedSensorValueLabel;
    QLabel *measureFunctionResultLabel;
    QLineEdit *minValueText;
    QLineEdit *maxValueText;

    QButtonGroup *fanPresetButtonGroup;

public slots:
    ///
    /// \brief updateControl applies the measure curve to the control associated
    ///
    void updateControl();

};

#endif // FANSYNCPAGE_H
