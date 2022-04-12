#include "FanSyncPage.h"

FanSyncPage::FanSyncPage(std::string cIdentifier, HardwareMonitor *hMonitor, QWidget *parent)
    : QWidget{parent}
{
    controlIdentifier = cIdentifier;
    hardwareMonitor = hMonitor;

    // main v layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    // first instruction lines
    QLabel *instructions = new QLabel("Add a measurement from the dropdown below then format the measurement function as required.\n"
                                      "Most Mathmatical expressions are supported, example: \"[/cpu/temp/0] + [/gpu/temp/0]) / 2\".\n"
                                      "See: http://www.partow.net/programming/exprtk/ for more info.");
    QFont instructionsFont( "Arial", 6);
    instructions->setFont(instructionsFont);
    mainLayout->addWidget(instructions);

    // hardware measures
    QHBoxLayout *hardwareDropdownLayout = new QHBoxLayout();

    sensorDropdown = new QComboBox();
    sensorDropdown->addItem("Select Sensor");
    sensorDropdown->setItemData(0, false, Qt::UserRole -1);

//    for (const auto& [sensorIdentifier, sensorName] : hardwareMonitor->sensorList)
//    {
//        sensorDropdown->setItemData(sensorDropdown->count() - 1, false, Qt::UserRole -1);
//        QString sensorIdentifierQstring = QString::fromStdString(sensorIdentifier);
//        QString sensorNameQstring = QString::fromStdString(sensorName);
//        sensorDropdown->addItem(sensorNameQstring, sensorIdentifierQstring);
//    }

    for (const auto& [hardwareName, type] : hardwareMonitor->sensorListHierarchy)
    {
        sensorDropdown->addItem(QString::fromStdString(hardwareName));
        sensorDropdown->setItemData(sensorDropdown->count() - 1, false, Qt::UserRole -1);
        for (const auto& [typeName, sensor] : type)
        {
            sensorDropdown->addItem(" " + QString::fromStdString(typeName));
            sensorDropdown->setItemData(sensorDropdown->count() - 1, false, Qt::UserRole -1);
            for (const auto& [sensorName, sensorIdentifier] : sensor)
            {
                sensorDropdown->addItem("    " + QString::fromStdString(sensorName), QString::fromStdString(sensorIdentifier));
            }
        }
    }


    hardwareDropdownLayout->addWidget(sensorDropdown, 3);

    selectedSensorValueLabel = new QLabel("");
    hardwareDropdownLayout->addWidget(selectedSensorValueLabel);

    // sensor dropdoown change action
    QObject::connect(sensorDropdown,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        if (!sensorDropdown->currentData().toBool())
        {
            return;
        }
        std::string sString = sensorDropdown->currentData().toString().toStdString();
        hardwareMonitor->sensorValue[sString] = hardwareMonitor->sensorValue[sString];
//        hardwareMonitor->updateSensors();

//        selectedSensorValueLabel->setText("(" + QString::number(hardwareMonitor->sensorValue[sString]) + ")");
    });

    QPushButton *addSensorButton = new QPushButton("Add");
    hardwareDropdownLayout->addWidget(addSensorButton, 1);

    mainLayout->addLayout(hardwareDropdownLayout);


    // measure function textbox
    measureFunctionTextEdit = new QPlainTextEdit();
    QObject::connect(measureFunctionTextEdit, &QPlainTextEdit::textChanged, this, &FanSyncPage::measureFunctionTextUpdated);

    mainLayout->addWidget(measureFunctionTextEdit, 2);

    // add sensor button action
    QObject::connect(addSensorButton, &QPushButton::clicked, this, [this]() {
        if (!sensorDropdown->currentData().toBool())
        {
            return;
        }
        measureFunctionTextEdit->insertPlainText("[" + sensorDropdown->currentData().toString() + "]");
    });

    // set min max
    QHBoxLayout *minMaxLayout = new QHBoxLayout();

    measureFunctionResultLabel = new QLabel("Result: nan | Normalized Result: nan");
    minMaxLayout->addWidget(measureFunctionResultLabel);

    minMaxLayout->addStretch();

    QValidator *intValidator = new QIntValidator(this);

    QLabel *minLabel = new QLabel("Min: ");
    minLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    minValueText = new QLineEdit("0");
    minValueText->setValidator(intValidator);
    minValueText->setFixedWidth(60);
    minMaxLayout->addWidget(minLabel);
    minMaxLayout->addWidget(minValueText);

    QLabel *maxLabel = new QLabel("Max: ");
    maxLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    maxValueText = new QLineEdit("100");
    maxValueText->setValidator(intValidator);
    maxValueText->setFixedWidth(60);
    minMaxLayout->addWidget(maxLabel);
    minMaxLayout->addWidget(maxValueText);

    mainLayout->addLayout(minMaxLayout);

    // Fan Curve
    fanCurveWidget = new FanCurvePlotWidget();
    mainLayout->addWidget(fanCurveWidget, 4);

    // Fan Presets
    QHBoxLayout *fanPresetLayout = new QHBoxLayout();

    fanPresetButtonGroup = new QButtonGroup();

    QRadioButton* customPresetButton = new QRadioButton("Custom");
    QRadioButton* silentPresetButton = new QRadioButton("Quiet");
    QRadioButton* normalPresetButton = new QRadioButton("Balanced");
    QRadioButton* performancePresetButton = new QRadioButton("Cool");
    QRadioButton* fullSpeedPresetButton = new QRadioButton("Full Speed");

    fanPresetButtonGroup->addButton(customPresetButton, 1);
    fanPresetButtonGroup->addButton(silentPresetButton, 2);
    fanPresetButtonGroup->addButton(normalPresetButton, 3);
    fanPresetButtonGroup->addButton(performancePresetButton, 4);
    fanPresetButtonGroup->addButton(fullSpeedPresetButton, 5);

    customPresetButton->setChecked(true);

    QObject::connect(fanPresetButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &FanSyncPage::setFanPreset);

    currentFanSpeed = new QLabel("Fan Speed: " + QString::number(hardwareMonitor->sensorValue[controlIdentifier]) + "%");
    fanPresetLayout->addWidget(currentFanSpeed);
    fanPresetLayout->addStretch();

    QLabel *fanPresetLabel = new QLabel("Presets: ");
    fanPresetLayout->addWidget(fanPresetLabel);

    fanPresetLayout->addWidget(customPresetButton);
    fanPresetLayout->addWidget(silentPresetButton);
    fanPresetLayout->addWidget(normalPresetButton);
    fanPresetLayout->addWidget(performancePresetButton);
    fanPresetLayout->addWidget(fullSpeedPresetButton);

    mainLayout->addLayout(fanPresetLayout);

    // Action buttons
    QHBoxLayout *actionButtonsLayout = new QHBoxLayout();
    actionButtonsLayout->addStretch();

    QPushButton *resetButton = new QPushButton("Reset");
    actionButtonsLayout->addWidget(resetButton);

    QObject::connect(resetButton, &QPushButton::clicked, this, &FanSyncPage::resetPage);


    QPushButton *saveButton = new QPushButton("Save");
    actionButtonsLayout->addWidget(saveButton);

    QObject::connect(saveButton, &QPushButton::clicked, this, &FanSyncPage::saveSettings);


    mainLayout->addLayout(actionButtonsLayout);

    loadSettings();
}

void FanSyncPage::setFanPreset(QAbstractButton *button)
{
    fanCurveWidget->qv_x.clear();
    fanCurveWidget->qv_y.clear();

    if (button->text() == "Quiet")
    {
        fanCurveWidget->qv_x.append(0);
        fanCurveWidget->qv_y.append(20);

        fanCurveWidget->qv_x.append(20);
        fanCurveWidget->qv_y.append(20);

        fanCurveWidget->qv_x.append(70);
        fanCurveWidget->qv_y.append(70);

        fanCurveWidget->qv_x.append(75);
        fanCurveWidget->qv_y.append(100);

        fanCurveWidget->qv_x.append(100);
        fanCurveWidget->qv_y.append(100);
    }
    else if (button->text() == "Balanced")
    {
        fanCurveWidget->qv_x.append(0);
        fanCurveWidget->qv_y.append(20);

        fanCurveWidget->qv_x.append(20);
        fanCurveWidget->qv_y.append(20);

        fanCurveWidget->qv_x.append(65);
        fanCurveWidget->qv_y.append(70);

        fanCurveWidget->qv_x.append(70);
        fanCurveWidget->qv_y.append(100);

        fanCurveWidget->qv_x.append(100);
        fanCurveWidget->qv_y.append(100);
    }
    else if (button->text() == "Cool")
    {
        fanCurveWidget->qv_x.append(0);
        fanCurveWidget->qv_y.append(20);

        fanCurveWidget->qv_x.append(20);
        fanCurveWidget->qv_y.append(20);

        fanCurveWidget->qv_x.append(55);
        fanCurveWidget->qv_y.append(70);

        fanCurveWidget->qv_x.append(60);
        fanCurveWidget->qv_y.append(100);

        fanCurveWidget->qv_x.append(100);
        fanCurveWidget->qv_y.append(100);
    }
    else if (button->text() == "Full Speed")
    {
        fanCurveWidget->qv_x.append(0);
        fanCurveWidget->qv_y.append(100);

        fanCurveWidget->qv_x.append(100);
        fanCurveWidget->qv_y.append(100);
    }
    else
    {
        fanCurveWidget->qv_x.append(0);
        fanCurveWidget->qv_y.append(0);

        fanCurveWidget->qv_x.append(100);
        fanCurveWidget->qv_y.append(100);
    }

    fanCurveWidget->updateData();
}

void FanSyncPage::measureFunctionTextUpdated()
{
    QString measureFunctionText = measureFunctionTextEdit->toPlainText();

    QStringList sensorStrings;
    extractSensorsFromString(measureFunctionText, sensorStrings);
    sensorStrings.removeDuplicates();

    if (sensorStrings.count() > 26)
    {
        QMessageBox Msgbox;
        Msgbox.setText("Too Many Sensors Used. Max is 26");
        Msgbox.exec();
        return;
    }

    measureFunctionExpressionSymbolTable = std::make_unique<exprtk::symbol_table<float>>();

    for (int i = 0; i < sensorStrings.count(); i++)
    {
        QString s = sensorStrings[i];

        std::string sString = s.toStdString();
        hardwareMonitor->sensorValue[sString] = hardwareMonitor->sensorValue[sString];

        measureFunctionText.replace(s, QString::fromStdString(variableNames[i]));

        measureFunctionExpressionSymbolTable->add_variable(variableNames[i], hardwareMonitor->sensorValue[sString]);
    }
    measureFunctionExpressionSymbolTable->add_constants();

    std::string measureFunctionTextString = measureFunctionText.toStdString();

    measureFunctionExpression = std::make_unique<exprtk::expression<float>>();
    measureFunctionExpression->register_symbol_table(*measureFunctionExpressionSymbolTable);

    exprtk::parser<float> parser;
    parser.compile(measureFunctionTextString, *measureFunctionExpression);
}

// adapted from https://www.geeksforgeeks.org/extract-substrings-between-any-pair-of-delimiters/
void FanSyncPage::extractSensorsFromString(QString str, QStringList &result)
{
    // Stores the indices of
    std::stack<int> dels;
    for (int i = 0; i < str.size(); i++) {
        // If opening delimiter
        // is encountered
        if (str[i] == '[') {
            dels.push(i);
        }

        // If closing delimiter
        // is encountered
        else if (str[i] == ']' && !dels.empty()) {

            // Extract the position
            // of opening delimiter
            int pos = dels.top();

            dels.pop();

            // Length of substring
            int len = i - 1 - pos;

            // Extract the substring
            QString ans = str.mid(pos + 1, len);

            result.push_back(ans);
        }
    }
}

void FanSyncPage::updateControl()
{
    float currentFanControlSpeed = hardwareMonitor->sensorValue[controlIdentifier];
    if (this->isVisible())
    {
        if (sensorDropdown->currentData().toBool())
        {
            selectedSensorValueLabel->setText("(" + QString::number(hardwareMonitor->sensorValue[sensorDropdown->currentData().toString().toStdString()]) + ")");
        }
        currentFanSpeed->setText("Fan Speed: " + QString::number(currentFanControlSpeed) + "%");
    }

    if (!measureFunctionExpression)
    {
        return;
    }

    float expressionResult = measureFunctionExpression->value();
    int minValue = minValueText->text().toInt();
    int maxValue = maxValueText->text().toInt();

    int normalizedResult = fmax(0, fmin(100, ceil((expressionResult - minValue) / (maxValue - minValue) * 100)));

    if(this->isVisible())
    {
        measureFunctionResultLabel->setText("Result: " + QString::number(expressionResult) + " | Normalized Result: " + QString::number(normalizedResult));
    }

    int fanSpeed = 100;

    int graphDataPointsCount = fanCurveWidget->getGraphData()->size();

    if (graphDataPointsCount > 1)
    {
        int x1Index = 0;
        int x2Index = 1;

        if (normalizedResult < fanCurveWidget->getGraphData()->at(0)->key) // point is below the first point
        {
            fanSpeed = fanCurveWidget->getGraphData()->at(0)->value;
        }
        else if (normalizedResult > fanCurveWidget->getGraphData()->at(graphDataPointsCount - 1)->key) // point is higher than the last point
        {
            fanSpeed = fanCurveWidget->getGraphData()->at(graphDataPointsCount - 1)->value;
        }
        else // check if we can place the point in between 2 points on the graph
        {
            for (int i = 1; i < graphDataPointsCount; i++)
            {
                if (normalizedResult >= fanCurveWidget->getGraphData()->at(i-1)->key && normalizedResult <= fanCurveWidget->getGraphData()->at(i)->key)
                {
                    x1Index = i - 1;
                    x2Index = i;
                    break;
                }
            }

            if (x1Index == -1 || x2Index == -1) // cant find anything give up and set fans to 100%
            {
                fanSpeed = 100;
            }
            else  // interpolate the fan speed based on the curve
            {
                double x1 = fanCurveWidget->getGraphData()->at(x1Index)->key;
                double y1 = fanCurveWidget->getGraphData()->at(x1Index)->value;
                double x2 = fanCurveWidget->getGraphData()->at(x2Index)->key;
                double y2 = fanCurveWidget->getGraphData()->at(x2Index)->value;

                fanSpeed = fmax(0, fmin(100, ceil((((y2 - y1) / (x2 - x1)) * (normalizedResult - x1)) + y1)));
            }
        }

        if (fanSpeed > currentFanControlSpeed + 2 || fanSpeed < currentFanControlSpeed - 2)
        {
            hardwareMonitor->setControlValue(controlIdentifier, fanSpeed);
        }
    }
}

void FanSyncPage::resetPage()
{
    sensorDropdown->setCurrentIndex(0);
    minValueText->setText("0");
    maxValueText->setText("100");
    measureFunctionTextEdit->setPlainText("");
    fanPresetButtonGroup->button(1)->click();
    measureFunctionExpression.reset();
    measureFunctionExpressionSymbolTable.reset();

    measureFunctionResultLabel->setText("Result: nan | Normalized Result: nan");
    selectedSensorValueLabel->setText("");
}

void FanSyncPage::saveSettings()
{
    json setting;
    setting["measureFunctionText"] = measureFunctionTextEdit->toPlainText().toStdString();
    setting["minValue"] = minValueText->text().toStdString();
    setting["maxValue"] = maxValueText->text().toStdString();
    setting["fanPreset"] = fanPresetButtonGroup->checkedId();
    setting["graphPointsX"] = json(fanCurveWidget->qv_x);
    setting["graphPointsY"] = json(fanCurveWidget->qv_y);
    Settings::Save(controlIdentifier, setting);
}

void FanSyncPage::loadSettings()
{
    json controlSettingJson = Settings::Load(controlIdentifier);

    if (controlSettingJson["minValue"].is_string())
    {
        minValueText->setText(QString::fromStdString(controlSettingJson["minValue"].get<std::string>()));
    }

    if (controlSettingJson["maxValue"].is_string())
    {
        maxValueText->setText(QString::fromStdString(controlSettingJson["maxValue"].get<std::string>()));
    }

    if (controlSettingJson["fanPreset"].is_number_integer())
    {
        fanPresetButtonGroup->button(controlSettingJson["fanPreset"].get<int>())->setChecked(true);
    }

    if (controlSettingJson["graphPointsX"].is_array() && controlSettingJson["graphPointsY"].is_array())
    {
        std::vector<double> graphX = controlSettingJson["graphPointsX"].get<std::vector<double>>();
        std::vector<double> graphY = controlSettingJson["graphPointsY"].get<std::vector<double>>();

        fanCurveWidget->qv_x = QVector<double>(graphX.begin(), graphX.end());
        fanCurveWidget->qv_y = QVector<double>(graphY.begin(), graphY.end());
        fanCurveWidget->updateData();
    }

    if (controlSettingJson["measureFunctionText"].is_string())
    {
        measureFunctionTextEdit->setPlainText(QString::fromStdString(controlSettingJson["measureFunctionText"].get<std::string>()));
    }
}
