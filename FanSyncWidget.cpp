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

#include "FanSyncWidget.h"
#include "OpenRGBFanSyncPlugin.h"

FanSyncWidget::FanSyncWidget(HardwareMonitor *hardwareMonitor, QWidget *parent)
    : QTabWidget{parent}
{
    this->setTabPosition(QTabWidget::West);

    loadSettings();

    // hide / show controls context menu
    this->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->tabBar(), &FanSyncWidget::customContextMenuRequested, this, &FanSyncWidget::tabBarContextMenuRequested);

    setUpdatesEnabled(false); // disable repainting till we are done

    bool firstPageSelected = false;
    for (const auto& [controlIdentifier, controlName] : hardwareMonitor->controlHardwareList)
    {
            FanSyncPage *page = new FanSyncPage(controlIdentifier, hardwareMonitor);

            QObject::connect(hardwareMonitor, &HardwareMonitor::sensorsUpdated, page, &FanSyncPage::updateControl);

            this->addTab(page, "");

            QString labelString;
            auto controlNamesIt = controlNames.find(controlIdentifier);
            if (controlNamesIt != controlNames.end())
            {
                labelString = QString::fromStdString(controlNamesIt->second);
            }
            else
            {
                labelString = QString::fromStdString(controlName);
            }

            Ui::TabLabel* tabLabel = new Ui::TabLabel("fan" + QString::fromStdString(OpenRGBFanSyncPlugin::DarkTheme ? "_dark" : ""), labelString);

            int tabIndex = this->tabBar()->count() - 1;

            this->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, tabLabel);
            this->tabBar()->setTabData(tabIndex, QString::fromStdString(controlIdentifier));

            if (std::find(hiddenControls.begin(), hiddenControls.end(), controlIdentifier) != hiddenControls.end())
            {
                this->tabBar()->setTabVisible(tabIndex, false);
            }
            else
            {
                if(!firstPageSelected)
                {
                    this->tabBar()->setCurrentIndex(tabIndex);
                    firstPageSelected = true;
                }
            }
    }

    setUpdatesEnabled(true); // reenable painting
}

void FanSyncWidget::tabBarContextMenuRequested(const QPoint& pos)
{
    QPoint globalPos = this->tabBar()->mapToGlobal(pos);
    int tabIndex = this->tabBar()->tabAt(pos);

    QMenu tabMenu;

    tabMenu.addAction("Rename");

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

        this->tabBar()->update();

        saveSettings();
    }
    else if (selectedItem && selectedItem->text() == "Show All")
    {
        setUpdatesEnabled(false); // disable repainting till we are done

        for(int i = 0; i < this->tabBar()->count(); i++)
        {
            if(!this->tabBar()->isTabVisible(i))
            {
                this->tabBar()->setTabVisible(i, true);
            }
        }

        setUpdatesEnabled(true); // reenable painting

        this->tabBar()->repaint();

        hiddenControls.clear();

        saveSettings();
    }
    else if (selectedItem && selectedItem->text() == "Rename")
    {
        bool inputOk;
        QString text = QInputDialog::getText(this, tr("Rename Control"), tr("Control name:"), QLineEdit::Normal, "", &inputOk);
        if (inputOk && !text.isEmpty()) {
            Ui::TabLabel* tabLabel = new Ui::TabLabel("fan" + QString::fromStdString(OpenRGBFanSyncPlugin::DarkTheme ? "_dark" : ""), text);

            this->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, tabLabel);
            controlNames[this->tabBar()->tabData(tabIndex).toString().toStdString()] = text.toStdString();

            this->tabBar()->update();

            saveSettings();
        }
    }
}

void FanSyncWidget::saveSettings()
{
    json hiddenControlsJson(hiddenControls);
    json controlNamesJson(controlNames);
    Settings::Save("hiddenControls", hiddenControlsJson);
    Settings::Save("controlNames", controlNamesJson);
}

void FanSyncWidget::loadSettings()
{
    json hiddenControlsJson = Settings::Load("hiddenControls");
    if (hiddenControlsJson.is_array())
    {
        hiddenControls = hiddenControlsJson.get<std::vector<std::string>>();
    }

    json controlNamesJson = Settings::Load("controlNames");
    if (controlNamesJson.is_object())
    {
        controlNames = controlNamesJson.get<std::map<std::string, std::string>>();
    }
}
