#include "FanSyncWidget.h"

FanSyncWidget::FanSyncWidget(HardwareMonitor *hardwareMonitor, QWidget *parent)
    : QTabWidget{parent}
{
    this->setTabPosition(QTabWidget::West);

    for (const auto& [controlIdentifier, controlName] : hardwareMonitor->controlHardwareList)
    {
            QWidget *page = new FanSyncPage(controlIdentifier, hardwareMonitor);
            this->addTab(page, "");

            QString labelString = QString::fromStdString(controlName);
            Ui::TabLabel* tabLabel = new Ui::TabLabel("fan", labelString);

            int tabIndex = this->tabBar()->count() - 1;

            this->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, tabLabel);
            this->tabBar()->setTabData(tabIndex, QString::fromStdString(controlIdentifier));

            if (hiddenControls.contains(controlIdentifier))
            {
                this->tabBar()->setTabVisible(tabIndex, false);
            }
    }

    // hide / show controls context menu
    this->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->tabBar(), &FanSyncWidget::customContextMenuRequested, this, &FanSyncWidget::tabBarContextMenuRequested);
}

void FanSyncWidget::tabBarContextMenuRequested(const QPoint& pos)
{
    QPoint globalPos = this->tabBar()->mapToGlobal(pos);
    int tabIndex = this->tabBar()->tabAt(pos);

    QMenu tabMenu;

    if (hiddenControls.count() < this->tabBar()->count() - 1)
    {
        tabMenu.addAction("Hide");
    }

    if (hiddenControls.count())
    {
        tabMenu.addAction("Show All");
    }

    QAction* selectedItem = tabMenu.exec(globalPos);
    if (selectedItem && selectedItem->text() == "Hide")
    {
        this->tabBar()->setTabVisible(tabIndex, false);
        hiddenControls.push_back(this->tabBar()->tabData(tabIndex).toString().toStdString());

        QWidget::repaint();
    }
    else if (selectedItem && selectedItem->text() == "Show All")
    {
        for(int i = 0; i < this->tabBar()->count(); i++)
        {
           this->tabBar()->setTabVisible(i, true);
        }
        hiddenControls.clear();

        QWidget::repaint();
    }
}
