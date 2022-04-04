#ifndef FANSYNCWIDGET_H
#define FANSYNCWIDGET_H

#include <QTabBar>
#include <QTabWidget>

#include <lhwm-cpp-wrapper.h>
#include "qt/TabLabel.h"

#include "FanSyncPage.h"

class FanSyncWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit FanSyncWidget(QWidget *parent = nullptr);

signals:

};

#endif // FANSYNCWIDGET_H
