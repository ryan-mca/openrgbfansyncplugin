#ifndef FANSYNCPAGE_H
#define FANSYNCPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>

#include "HardwareMonitor.h"
#include "FanCurvePlotWidget.h"

class FanSyncPage : public QWidget
{
    Q_OBJECT
public:
    FanSyncPage(std::string controlIdentifier, HardwareMonitor *hardwareMonitor);

private:
    QComboBox *sensorDropdown;
    QPlainTextEdit *measureFunctionText;
    QLabel *selectedSensorValueLabel;
    QLabel *measureFunctionResultLabel;
    QLineEdit *minValueText;
    QLineEdit *maxValueText;

    QButtonGroup *fanPresetButtonGroup;
};

#endif // FANSYNCPAGE_H
