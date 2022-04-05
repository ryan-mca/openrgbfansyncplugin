#include "FanSyncWidget.h"

FanSyncWidget::FanSyncWidget(HardwareMonitor *hardwareMonitor)
    : QTabWidget{nullptr}
{
    this->setTabPosition(QTabWidget::West);

    for (const auto& [controlIdentifier, controlName] : hardwareMonitor->ControlHardwareList)
    {
            QWidget *page = new FanSyncPage(controlIdentifier, hardwareMonitor);
            this->addTab(page, "");

            QString labelString = QString::fromStdString(controlName);
            Ui::TabLabel* tabLabel = new Ui::TabLabel("fan", labelString);

            this->tabBar()->setTabButton(this->tabBar()->count() - 1, QTabBar::LeftSide, tabLabel);
    }
}
