#include "FanSyncWidget.h"

FanSyncWidget::FanSyncWidget(QWidget *parent)
    : QTabWidget{parent}
{
    this->setTabPosition(QTabWidget::West);

    std::map<std::string, std::vector<std::tuple<std::string, std::string, std::string>>>
                hs_map = LHWM::GetHardwareSensorMap();

    for (const auto& [HardwareName, HardwareSensors] : hs_map)
    {
        for (const auto& hardwareSensor : HardwareSensors)
        {
            if(std::get<1>(hardwareSensor) != "Control")
            {
                continue;
            }

            QWidget *page = new FanSyncPage();
            this->addTab(page, "");

            QString labelString = QString::fromStdString(std::get<0>(hardwareSensor));
            Ui::TabLabel* tabLabel = new Ui::TabLabel("fan", labelString);

            this->tabBar()->setTabButton(this->tabBar()->count() - 1, QTabBar::LeftSide, tabLabel);
        }
    }
}
