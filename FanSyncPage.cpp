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
                                      "Most Mathmatical expressions are supported, example: \"({{cputemp}} + {{gputemp}}) / 2\".\n"
                                      "See: http://www.partow.net/programming/exprtk/ for more info.");
    QFont instructionsFont( "Arial", 6);
    instructions->setFont(instructionsFont);
    mainLayout->addWidget(instructions);

    // hardware measures
    QHBoxLayout *hardwareDropdownLayout = new QHBoxLayout();

    sensorDropdown = new QComboBox();

    for (const auto& [sensorIdentifier, sensorName] : hardwareMonitor->sensorList)
    {
        QString sensorIdentifierQstring = QString::fromStdString(sensorIdentifier);
        QString sensorNameQstring = QString::fromStdString(sensorName);
        sensorDropdown->addItem(sensorNameQstring, sensorIdentifierQstring);
    }

    hardwareDropdownLayout->addWidget(sensorDropdown, 3);

    selectedSensorValueLabel = new QLabel("()");
    hardwareDropdownLayout->addWidget(selectedSensorValueLabel);

    QPushButton *addSensorButton = new QPushButton("Add");
    hardwareDropdownLayout->addWidget(addSensorButton, 1);

//    QLabel *instructionsToolTipLabel = new QLabel("i");
//    instructionsToolTipLabel->setToolTip("Add a measurement from the dropdown on the left then format the measurement function as required.\n"
//                                         "Most Mathmatical expressions are supported, example: \"({{cputemp}} + {{gputemp}}) / 2\".\n"
//                                         "See: http://www.partow.net/programming/exprtk/ for more info.");
//    hardwareDropdownLayout->addWidget(instructionsToolTipLabel);

    mainLayout->addLayout(hardwareDropdownLayout);


    // measure function textbox
    measureFunctionText = new QPlainTextEdit();
    mainLayout->addWidget(measureFunctionText, 2);

    // add sensor button action
    QObject::connect(addSensorButton, &QPushButton::clicked, addSensorButton, [this]() {
        if (sensorDropdown->currentIndex() == -1)
        {
            return;
        }
        measureFunctionText->appendPlainText("{{" + sensorDropdown->currentData().toString() + "}}");
    });

    // set min max
    QHBoxLayout *minMaxLayout = new QHBoxLayout();

    measureFunctionResultLabel = new QLabel("Current Value: 0");
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
    FanCurvePlotWidget *fanCurveWidget = new FanCurvePlotWidget();
    mainLayout->addWidget(fanCurveWidget, 4);

    // Fan Presets
    QHBoxLayout *fanPresetLayout = new QHBoxLayout();

    fanPresetButtonGroup = new QButtonGroup();

    QRadioButton* customPresetButton = new QRadioButton("Custom");
    QRadioButton* silentPresetButton = new QRadioButton("Silent");
    QRadioButton* normalPresetButton = new QRadioButton("Normal");
    QRadioButton* performancePresetButton = new QRadioButton("Performance");

    fanPresetButtonGroup->addButton(customPresetButton, 1);
    fanPresetButtonGroup->addButton(silentPresetButton, 2);
    fanPresetButtonGroup->addButton(normalPresetButton, 3);
    fanPresetButtonGroup->addButton(performancePresetButton, 4);

    customPresetButton->setChecked(true);

    QLabel *fanPresetLabel = new QLabel("Preset: ");
    fanPresetLayout->addWidget(fanPresetLabel);

    fanPresetLayout->addWidget(customPresetButton);
    fanPresetLayout->addWidget(silentPresetButton);
    fanPresetLayout->addWidget(normalPresetButton);
    fanPresetLayout->addWidget(performancePresetButton);

    mainLayout->addLayout(fanPresetLayout);

    // Action buttons
    QHBoxLayout *actionButtonsLayout = new QHBoxLayout();
    actionButtonsLayout->addStretch();

    QPushButton *resetButton = new QPushButton("Reset");
    actionButtonsLayout->addWidget(resetButton);

    QPushButton *saveButton = new QPushButton("Save");
    actionButtonsLayout->addWidget(saveButton);

    mainLayout->addLayout(actionButtonsLayout);

    connect(saveButton, &QPushButton::released, this, &FanSyncPage::updateControl);

}

void FanSyncPage::updateControl()
{
    typedef exprtk::symbol_table<float> symbol_table_t;
    typedef exprtk::expression<float>   expression_t;
    typedef exprtk::parser<float>       parser_t;

    std::string expression_string = measureFunctionText->toPlainText().toStdString();

    float x;

    symbol_table_t symbol_table;
    symbol_table.add_variable("{{xe}}",x);
    symbol_table.add_constants();

    expression_t expression;
    expression.register_symbol_table(symbol_table);

    parser_t parser;
    parser.compile(expression_string,expression);

    for (x = float(-5); x <= float(+5); x += float(1))
    {
       float y = expression.value();
       printf("%19.15f\t%19.15f\n",x,y);
    }
}


