#include "FanSyncPage.h"

FanSyncPage::FanSyncPage(QWidget *parent)
    : QWidget{parent}
{
    // main v layout
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    // first instruction lines
    QLabel *instructionsLine1 = new QLabel("Add a measurement from the dropdown below then format the measurement function as required.");
    QLabel *instructionsLine2 = new QLabel("Most Mathmatical expressions are supported, example: \"({{cputemp}} + {{gputemp}}) / 2\". \n See: http://www.partow.net/programming/exprtk/ for more info.");

    layout->addWidget(instructionsLine1, 1);
    layout->addWidget(instructionsLine2, 1);

    // hardware measures
}
