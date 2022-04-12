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

#ifndef FANSYNCWIDGET_H
#define FANSYNCWIDGET_H

#include <QTabBar>
#include <QTabWidget>

#include "qt/TabLabel.h"

#include "HardwareMonitor.h"
#include "FanSyncPage.h"
#include "Settings.h"

class FanSyncWidget : public QTabWidget
{
    Q_OBJECT
public:
    FanSyncWidget(HardwareMonitor *hardwareMonitor, QWidget *parent = nullptr);

private:
    std::vector<std::string> hiddenControls;
    void saveSettings();
    void loadSettings();

private slots:
    void tabBarContextMenuRequested(const QPoint& pos);

};

#endif // FANSYNCWIDGET_H
