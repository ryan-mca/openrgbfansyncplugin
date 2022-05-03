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
    QLabel *currentFanDutyCycle;
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
