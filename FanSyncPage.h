#ifndef FANSYNCPAGE_H
#define FANSYNCPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVector>

#include <exprtk.hpp>

#include "HardwareMonitor.h"
#include "FanCurvePlotWidget.h"
#include "Settings.h"

class FanSyncPage : public QWidget
{
    Q_OBJECT
public:
    FanSyncPage(std::string controlIdentifier, HardwareMonitor *hardwareMonitor, QWidget *parent = nullptr);

private:
    const std::string variableNames[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

    std::string controlIdentifier;
    HardwareMonitor *hardwareMonitor;


    QComboBox *sensorDropdown;
    QPlainTextEdit *measureFunctionTextEdit;
    QLabel *selectedSensorValueLabel;
    QLabel *measureFunctionResultLabel;
    QLineEdit *minValueText;
    QLineEdit *maxValueText;
    QLabel *currentFanSpeed;
    QButtonGroup *fanPresetButtonGroup;

    FanCurvePlotWidget *fanCurveWidget;

    std::unique_ptr<exprtk::expression<float>> measureFunctionExpression;
    std::unique_ptr<exprtk::symbol_table<float>> measureFunctionExpressionSymbolTable;

    void extractSensorsFromString(QString str, QStringList &result);

    void resetPage();
    void saveSettings();
    void loadSettings();

public slots:
    ///
    /// \brief updateControl applies the measure curve to the control associated
    ///
    void updateControl();

private slots:
    ///
    /// \brief measureFunctionTextUpdated called when the measure function text is updated
    /// this handles creating the exprtk expression and replacing the sensor ids with single letter variables
    ///
    void measureFunctionTextUpdated();

    void setFanPreset(QAbstractButton *button);

};

#endif // FANSYNCPAGE_H
