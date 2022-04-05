#ifndef FANSYNCPAGE_H
#define FANSYNCPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>

#include "HardwareMonitor.h"

class FanSyncPage : public QWidget
{
    Q_OBJECT
public:
    FanSyncPage(std::string controlIdentifier, HardwareMonitor *hardwareMonitor);

signals:

};

#endif // FANSYNCPAGE_H
