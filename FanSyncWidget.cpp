#include "FanSyncWidget.h"
#include "OpenRGBFanSyncPlugin.h"

FanSyncWidget::FanSyncWidget(HardwareMonitor *hardwareMonitor, QWidget *parent)
    : QTabWidget{parent}
{
    this->setTabPosition(QTabWidget::West);

    loadSettings();

    for (const auto& [controlIdentifier, controlName] : hardwareMonitor->controlHardwareList)
    {
            FanSyncPage *page = new FanSyncPage(controlIdentifier, hardwareMonitor);

            QObject::connect(hardwareMonitor, &HardwareMonitor::sensorsUpdated, page, &FanSyncPage::updateControl);

            this->addTab(page, "");

            QString labelString = QString::fromStdString(controlName);
            Ui::TabLabel* tabLabel = new Ui::TabLabel("fan" + QString::fromStdString(OpenRGBFanSyncPlugin::DarkTheme ? "_dark" : ""), labelString);

            int tabIndex = this->tabBar()->count() - 1;

            this->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, tabLabel);
            this->tabBar()->setTabData(tabIndex, QString::fromStdString(controlIdentifier));

            if (std::find(hiddenControls.begin(), hiddenControls.end(), controlIdentifier) != hiddenControls.end())
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

    if (hiddenControls.size() < this->tabBar()->count() - 1)
    {
        tabMenu.addAction("Hide");
    }

    if (hiddenControls.size())
    {
        tabMenu.addAction("Show All");
    }

    QAction* selectedItem = tabMenu.exec(globalPos);
    if (selectedItem && selectedItem->text() == "Hide")
    {
        this->tabBar()->setTabVisible(tabIndex, false);
        hiddenControls.push_back(this->tabBar()->tabData(tabIndex).toString().toStdString());

        QWidget::repaint();

        saveSettings();
    }
    else if (selectedItem && selectedItem->text() == "Show All")
    {
        for(int i = 0; i < this->tabBar()->count(); i++)
        {
           this->tabBar()->setTabVisible(i, true);
        }
        hiddenControls.clear();

        QWidget::repaint();

        saveSettings();
    }
}

void FanSyncWidget::saveSettings()
{
    json value(hiddenControls);
    Settings::Save("hiddenControls", value);
}

void FanSyncWidget::loadSettings()
{
    json hiddenControlsJson = Settings::Load("hiddenControls");
    if (hiddenControlsJson.is_array())
    {
        hiddenControls = hiddenControlsJson.get<std::vector<std::string>>();
    }
}
