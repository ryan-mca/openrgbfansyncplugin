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
