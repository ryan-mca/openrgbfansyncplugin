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

            int tabIndex = this->tabBar()->count() - 1;

            this->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, tabLabel);

            if (hiddenControls.contains(controlIdentifier))
            {
                this->tabBar()->setTabVisible(tabIndex, false);
            }

            // hide / show controls context menu
            tabLabel->setContextMenuPolicy(Qt::CustomContextMenu);
            QObject::connect(tabLabel, &FanSyncWidget::customContextMenuRequested, this, [this, tabLabel, tabIndex](const QPoint& pos) {
                QPoint globalPos = tabLabel->mapToGlobal(pos);
                QMenu tabMenu;
                tabMenu.addAction("Hide");
                tabMenu.addAction("Show All");

                QAction* selectedItem = tabMenu.exec(globalPos);
                if (selectedItem && selectedItem->text() == "Hide")
                {
                    this->tabBar()->setTabVisible(tabIndex, false);
                    QWidget::repaint();
                }
                else if (selectedItem && selectedItem->text() == "Show All")
                {
                    for(int i = 0; i < this->tabBar()->count(); i++)
                    {
                       this->tabBar()->setTabVisible(i, true);
                    }
                    QWidget::repaint();
                }
            });
    }
}
