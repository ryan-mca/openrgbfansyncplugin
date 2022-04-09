#ifndef FANSYNCWIDGET_H
#define FANSYNCWIDGET_H

#include <QTabBar>
#include <QTabWidget>

#include "qt/TabLabel.h"

#include "HardwareMonitor.h"
#include "FanSyncPage.h"

class FanSyncWidget : public QTabWidget
{
    Q_OBJECT
public:
    FanSyncWidget(HardwareMonitor *hardwareMonitor);

private:
    QList<std::string> hiddenControls;

};

#endif // FANSYNCWIDGET_H
