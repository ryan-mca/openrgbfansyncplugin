#ifndef FANCURVEPLOTWIDGET_H
#define FANCURVEPLOTWIDGET_H

#include <QWidget>

#include <qcustomplot.h>

class FanCurvePlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FanCurvePlotWidget(QWidget *parent = nullptr);
    QVector<double> qv_x, qv_y;
    QVector<int> cpoints_x, cpoints_y;

signals:
    void on_clickedPoint(QMouseEvent *event);
    void on_dragPoint(bool);
    void on_clickedGraph(bool);

private slots:
    void clickedGraph(QMouseEvent *event);
    void rePlot();
    void clickedPoint(QCPAbstractPlottable*, int, QMouseEvent *event);
    bool initializeDragging(QMouseEvent *event);
    bool detectMove(QMouseEvent *event);
    bool detectPress(QMouseEvent *event);
    void detectRelease(QMouseEvent*);
    bool checkForDuplicatePoint(int x, int y);
    bool removePoint(int x, int y);
    void getClosestCoords(QMouseEvent *event);
    void getPointIndices();
    bool checkForNearbyPoints(QMouseEvent *event);
    void dragPoint(int index_x, int index_y, QMouseEvent *event);
    void drawFillerLines();
    void drawPointHoveredText(QMouseEvent *event);

private:
    QVector<double> leftLineX;
    QVector<double> leftLineY;
    QVector<double> rightLineX;
    QVector<double> rightLineY;
    QVector<int> curvepoints;
    QPair<int, int> curvepoint;
    QCPItemText *coordText;
    int x_lower = 0;
    int x_upper = 100;
    int y_lower = 0;
    int y_upper = 100;
    double pixelLength;
    double selectionPrecision = 2;
    bool mouseMoving = false;
    bool mousePressed = false;
    bool mouseDragging = false;
    bool duplicatePoint = false;
    bool isNearbyPoint = false;
    bool coordTextCreated = false;
    int pointIndex;
    int y_length;
    int x_length;
    int xcoord;
    int ycoord;
    int index_x = 0;
    int index_y = 0;
    bool indicesSet = false;
    bool draggingPoint = false;
    QTimer *pressTimer = new QTimer(this);
    int timerTime = 1000;

    QCustomPlot* fanCurve;
};

#endif // FANCURVEPLOTWIDGET_H
