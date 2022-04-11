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
    FanSyncWidget(HardwareMonitor *hardwareMonitor, bool darkTheme, QWidget *parent = nullptr);

private:
    QList<std::string> hiddenControls;

private slots:
    void tabBarContextMenuRequested(const QPoint& pos);

};

#endif // FANSYNCWIDGET_H
