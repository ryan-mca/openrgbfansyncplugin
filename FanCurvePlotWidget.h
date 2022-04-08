/*
    Major portions of this file were adapted from TuxClocker (editprofile.h).

    https://github.com/Lurkki14/tuxclocker/blob/master/editprofile.h

    Copyright (c) 2019 Jussi Kuokkanen

    Modified by Shady Nawara, 2022
*/

#ifndef FANCURVEPLOTWIDGET_H
#define FANCURVEPLOTWIDGET_H

#include <QWidget>

#include <qcustomplot.h>

class FanCurvePlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FanCurvePlotWidget(QWidget *parent = nullptr);

    ///
    /// \brief qv_x Curve points X coords, length must match qv_y
    ///
    QVector<double> qv_x;

    ///
    /// \brief qv_y Curve points Y coords, length must match qv_x
    ///
    QVector<double> qv_y;

    ///
    /// \brief setXAxisRange changes the X axis min/max range
    /// \param min
    /// \param max
    /// \param offset value to add to the max axis range for visibility
    ///
    void setXAxisRange(int min, int max, int maxMargin);

    ///
    /// \brief setYAxisRange changes the Y axis min/max range
    /// \param min
    /// \param max
    /// \param offset value to add to the max axis range for visibility
    ///
    void setYAxisRange(int min, int max, int maxMargin);

    ///
    /// \brief setXAxisLabel changes the X Axis Label
    /// \param text
    ///
    void setXAxisLabel(QString& text);

    ///
    /// \brief setYAxisLabel changes the Y Axis Label
    /// \param text
    ///
    void setYAxisLabel(QString& text);

    ///
    /// \brief updateData needs to be called after changing the qv_x and qv_y array
    ///
    void updateData();

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
    QCPItemText *coordText;
    int x_lower = 0;
    int x_upper = 100;
    int y_lower = 0;
    int y_upper = 100;
    double selectionPrecision = 2;
    bool mouseMoving = false;
    bool mousePressed = false;
    bool mouseDragging = false;
    bool duplicatePoint = false;
    bool isNearbyPoint = false;
    int xcoord;
    int ycoord;
    int index_x = 0;
    int index_y = 0;
    bool draggingPoint = false;
    QTimer *pressTimer = new QTimer(this);

    QCustomPlot* fanCurve;
};

#endif // FANCURVEPLOTWIDGET_H
