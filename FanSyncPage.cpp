#include "FanSyncPage.h"

FanSyncPage::FanSyncPage(std::string controlIdentifier, HardwareMonitor *hardwareMonitor)
    : QWidget{nullptr}
{
    // main v layout
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    // first instruction lines
    QLabel *instructions = new QLabel("Add a measurement from the dropdown below then format the measurement function as required.\n"
                                      "Most Mathmatical expressions are supported, example: \"({{cputemp}} + {{gputemp}}) / 2\".\n"
                                      "See: http://www.partow.net/programming/exprtk/ for more info.");

    layout->addWidget(instructions, 1);

    // hardware measures
    QHBoxLayout *hardwareDropdownLayout = new QHBoxLayout();

    QComboBox *sensorDropdown = new QComboBox();

    for (const auto& [sensorIdentifier, sensorName] : hardwareMonitor->SensorList)
    {
        QString sensorIdentifierQstring = QString::fromStdString(sensorIdentifier);
        QString sensorNameQstring = QString::fromStdString(sensorName);
        sensorDropdown->addItem(sensorNameQstring, sensorIdentifierQstring);
    }

    hardwareDropdownLayout->addWidget(sensorDropdown, 3);

    QPushButton *addSensorButton = new QPushButton("Add");
    hardwareDropdownLayout->addWidget(addSensorButton, 1);

    layout->addLayout(hardwareDropdownLayout, 1);


    // measure function textbox
    QPlainTextEdit *measureFunctionText = new QPlainTextEdit();

    layout->addWidget(measureFunctionText, 2);

    // add sensor button action
    QObject::connect(addSensorButton, &QPushButton::clicked, addSensorButton, [=]() {
        if (sensorDropdown->currentIndex() == -1)
        {
            return;
        }
        measureFunctionText->appendPlainText("{{" + sensorDropdown->currentData().toString() + "}}");
    });


}
