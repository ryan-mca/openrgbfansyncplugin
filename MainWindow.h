#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout *layout;
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H
