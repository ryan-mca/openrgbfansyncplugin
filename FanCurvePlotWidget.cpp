/*
    Major portions of this file were adapted from TuxClocker (editprofile.h).

    https://github.com/Lurkki14/tuxclocker/blob/master/editprofile.h

    Copyright (c) 2019 Jussi Kuokkanen

    Modified by Shady Nawara, 2022
*/

#include "FanCurvePlotWidget.h"

FanCurvePlotWidget::FanCurvePlotWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* layout = new QVBoxLayout();
    this->setLayout(layout);

    fanCurve = new QCustomPlot();

    layout->addWidget(fanCurve);

    QPalette palette;
    palette.setCurrentColorGroup(QPalette::Active);
    QColor color = palette.color(QPalette::Window);
    QColor textColor = palette.color(QPalette::Text);
    QColor graphColor = palette.color(QPalette::Highlight);
    QPen graphPen;
    graphPen.setWidth(2);
    graphPen.setColor(graphColor);
    QPen tickPen;
    tickPen.setWidthF(0.5);
    tickPen.setColor(textColor);

    // Define the filler line vectors and graphs so they don't need to be recreated every update
    leftLineX.append(x_lower);
    leftLineX.append(0);
    leftLineY.append(0);
    leftLineY.append(0);

    rightLineX.append(0);
    rightLineX.append(x_upper);
    rightLineY.append(0);
    rightLineY.append(0);

    fanCurve->addGraph();
    fanCurve->addGraph();

    fanCurve->addGraph();
    fanCurve->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    fanCurve->graph(0)->setLineStyle(QCPGraph::lsLine);

    fanCurve->setBackground(color);
    fanCurve->xAxis->setLabelColor(textColor);
    fanCurve->yAxis->setLabelColor(textColor);
    fanCurve->xAxis->setTickLabelColor(textColor);
    fanCurve->yAxis->setTickLabelColor(textColor);
    fanCurve->graph(0)->setPen(graphPen);
    fanCurve->graph(1)->setPen(graphPen);
    fanCurve->graph(2)->setPen(graphPen);
    fanCurve->xAxis->setTickPen(tickPen);
    fanCurve->yAxis->setTickPen(tickPen);
    fanCurve->xAxis->setSubTickPen(tickPen);
    fanCurve->yAxis->setSubTickPen(tickPen);
    fanCurve->xAxis->setBasePen(tickPen);
    fanCurve->yAxis->setBasePen(tickPen);



    connect(fanCurve, SIGNAL(mouseDoubleClick(QMouseEvent*)), SLOT(clickedGraph(QMouseEvent*)));
    connect(fanCurve, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), SLOT(clickedPoint(QCPAbstractPlottable*,int,QMouseEvent*)));
    connect(fanCurve, SIGNAL(mousePress(QMouseEvent*)), SLOT(detectPress(QMouseEvent*)));
    connect(fanCurve, SIGNAL(mouseMove(QMouseEvent*)), SLOT(detectMove(QMouseEvent*)));
    connect(fanCurve, SIGNAL(mouseRelease(QMouseEvent*)), SLOT(detectRelease(QMouseEvent*)));
    connect(fanCurve, SIGNAL(mouseMove(QMouseEvent*)), SLOT(getClosestCoords(QMouseEvent*)));
    connect(fanCurve, SIGNAL(mouseMove(QMouseEvent*)), SLOT(drawPointHoveredText(QMouseEvent*)));

    qv_x.append(0);
    qv_y.append(0);

    qv_x.append(100);
    qv_y.append(100);

    fanCurve->xAxis->setRange(x_lower, x_upper + 5);
    fanCurve->yAxis->setRange(y_lower, y_upper + 10);

    fanCurve->xAxis->setLabel("Measure Value");
    fanCurve->yAxis->setLabel("Fan speed (%)");

    fanCurve->graph(0)->setData(qv_x, qv_y);
    drawFillerLines();

    QCPItemText *text = new QCPItemText(fanCurve);
    coordText = text;
    coordText->setColor(textColor);
}

void FanCurvePlotWidget::rePlot()
{
    fanCurve->replot();
    fanCurve->update();
}

void FanCurvePlotWidget::clickedGraph(QMouseEvent *event)
{
    QPoint point = event->pos();

    int x = round(fanCurve->xAxis->pixelToCoord(point.x()));
    int y = round(fanCurve->yAxis->pixelToCoord(point.y()));
    if (qv_x.length() != 0) {
        checkForDuplicatePoint(x, y);
    }
    if (duplicatePoint)
    {
        removePoint(x, y);
        index_y = qv_y.size()-1;
        index_x = qv_x.size()-1;
    }
    else
    {
        if ((x_lower<=x) && (x<=x_upper) && (y_lower<=y) && (y<=y_upper)) {
            qv_x.append(x);
            qv_y.append(y);
            index_y = qv_y.size()-1;
            index_x = qv_x.size()-1;
        }
    }
    fanCurve->graph(0)->setData(qv_x, qv_y);
    drawFillerLines();
}

void FanCurvePlotWidget::clickedPoint(QCPAbstractPlottable*, int, QMouseEvent *event)
{
    checkForNearbyPoints(event);
    ycoord = round(ycoord);
    xcoord = round(xcoord);
    if (isNearbyPoint && qv_x.length() != 0) {
            for (int i=0; i<qv_y.length(); i++ ) {
                qv_y[i] = round(qv_y[i]);
                qv_x[i] = round(qv_x[i]);
                if ((qv_x[i] == xcoord) && (qv_y[i] == ycoord)) {
                    qv_x.remove(i);
                    qv_y.remove(i);
                    break;
                }
            }
      fanCurve->graph(0)->setData(qv_x, qv_y);
      rePlot();
      drawFillerLines();
    }
}

bool FanCurvePlotWidget::checkForNearbyPoints(QMouseEvent *event)
{
    if (qv_x.length() != 0) {
        QPoint point = event->pos();
        double pointerxcoord = fanCurve->xAxis->pixelToCoord(point.x());
        double pointerycoord = fanCurve->yAxis->pixelToCoord(point.y());
        for (int i=0; i<qv_x.length(); i++) {
            if ((abs(pointerxcoord - qv_x[i]) < selectionPrecision) && abs(pointerycoord - qv_y[i]) < selectionPrecision) {
                isNearbyPoint = true;
                break;
            } else {
                isNearbyPoint = false;
            }
        }
    return isNearbyPoint;
    }
    return false;
}

void FanCurvePlotWidget::drawPointHoveredText(QMouseEvent *event)
{
    checkForNearbyPoints(event);
    if (isNearbyPoint && !draggingPoint) {
        if (xcoord < x_upper*0.1) {
            coordText->position->setCoords(x_upper*0.1, xcoord + 4);
        } else if (xcoord > x_upper*0.9) {
            coordText->position->setCoords(x_upper*0.9, xcoord + 4);
        } else {
            coordText->position->setCoords(xcoord, ycoord + 4);
        }

        coordText->setText(QString::number(xcoord) + ", " + QString::number(ycoord));
        rePlot();
    }
    if (!isNearbyPoint && !draggingPoint) {
        coordText->setText("");
        rePlot();
    }
}

bool FanCurvePlotWidget::checkForDuplicatePoint(int x, int y)
// Return true if there is a duplicate point
{
    for (int i=0; i<qv_x.length(); i++) {
        qv_x[i] = round(qv_x[i]);
        qv_y[i] = round(qv_y[i]);
        if ((x == qv_x[i]) && (y == qv_y[i])) {
            duplicatePoint = true;
            return duplicatePoint;
        }
    }
    duplicatePoint = false;
    return duplicatePoint;
}

bool FanCurvePlotWidget::removePoint(int x, int y)
// Return true if there is a duplicate point
{
    for (int i=0; i<qv_x.length(); i++) {
        qv_x[i] = round(qv_x[i]);
        qv_y[i] = round(qv_y[i]);
        if ((x == qv_x[i]) && (y == qv_y[i])) {
            qv_x.remove(i);
            qv_y.remove(i);
            return true;
        }
    }
    return false;
}

void FanCurvePlotWidget::getClosestCoords(QMouseEvent *event)
{
    // Get the coordinates of the point that the mouse is over
    if (qv_y.size() > 0) {
        QPoint cursor = event->pos();
        double pointerycoord = fanCurve->yAxis->pixelToCoord(cursor.y());
        double pointerxcoord = fanCurve->xAxis->pixelToCoord(cursor.x());
        for (int i=0; i<qv_y.size(); i++) {
            if (sqrt(abs(pointerxcoord - qv_x[i]) * (abs(pointerxcoord - qv_x[i])) + abs(pointerycoord - qv_y[i]) * abs(pointerycoord - qv_y[i])) < selectionPrecision) {
                xcoord = qv_x[i];
                ycoord = qv_y[i];
                break;
            }
        }
    }
    getPointIndices();
}

bool FanCurvePlotWidget::detectMove(QMouseEvent *event)
{
    mouseMoving = true;
    if (mouseMoving && mousePressed) {
        initializeDragging(event);
    }
    return mouseMoving;
}

bool FanCurvePlotWidget::detectPress(QMouseEvent *event)
{
    mousePressed = true;
    if (mouseMoving && mousePressed) {
        initializeDragging(event);
    }
    return mousePressed;
}

bool FanCurvePlotWidget::initializeDragging(QMouseEvent *event)
{
    // Decides whether to start dragging the point
    if (!pressTimer->isActive() && !mouseDragging) {
        pressTimer->start(20);
        pressTimer->setSingleShot(true);
    }
    mouseDragging = true;
    checkForNearbyPoints(event);
    if ((isNearbyPoint || draggingPoint) && pressTimer->remainingTime() <= 0) {
        dragPoint(index_x, index_y, event);
        draggingPoint = true;
    }
    return mouseDragging;
}

void FanCurvePlotWidget::getPointIndices()
{
    if (qv_y.size() > 0) {
        for (int i=0; i<qv_y.size(); i++) {
            if ((qv_x[i] == xcoord) && (qv_y[i] == ycoord)) {
                index_x = i;
                index_y = i;
                break;
            }
        }
    }
}

void FanCurvePlotWidget::dragPoint(int index_x, int index_y, QMouseEvent* event)
{
    // Moves the selected point by index
    // Sleep here so we don't take up so much CPU time
    QThread::msleep(10);
    QPoint point = event->pos();
    qv_y[index_y] = round(fanCurve->yAxis->pixelToCoord(point.y()));
    qv_x[index_x] = round(fanCurve->xAxis->pixelToCoord(point.x()));

    if (qv_x[index_x] > x_upper) {
        qv_x[index_x] = x_upper;
    }
    if (qv_x[index_x] < x_lower) {
        qv_x[index_x] = x_lower;
    }
    if (qv_y[index_y] > y_upper) {
        qv_y[index_y] = y_upper;
    }
    if (qv_y[index_y] < y_lower) {
        qv_y[index_y] = y_lower;
    }
    // Display the coordinates
    if (fanCurve->xAxis->pixelToCoord(point.x()) < x_upper*0.1) {
        coordText->position->setCoords(x_upper*0.1, qv_y[index_y] + 4);
    } else if (fanCurve->xAxis->pixelToCoord(point.x()) > x_upper*0.9) {
        coordText->position->setCoords(x_upper*0.9, qv_y[index_y] + 4);
    } else {
        coordText->position->setCoords(qv_x[index_x], qv_y[index_y] + 4);
    }
    QString xString = QString::number(qv_x[index_x]);
    QString yString = QString::number(qv_y[index_y]);
    coordText->setText(xString + ", " + yString);

    fanCurve->graph(0)->setData(qv_x, qv_y);
    drawFillerLines();
}

void FanCurvePlotWidget::drawFillerLines()
{
    // Draw the filler lines separately so they don't interfere with the main data. graph(1) = leftward line, graph(2) = rightward line

    leftLineX[1] = fanCurve->graph(0)->dataSortKey(0);

    leftLineY[0] = fanCurve->graph(0)->dataMainValue(0);
    leftLineY[1] = fanCurve->graph(0)->dataMainValue(0);

    fanCurve->graph(1)->setData(leftLineX, leftLineY);

    rightLineX[0] = fanCurve->graph(0)->dataSortKey(qv_x.length() -1);

    rightLineY[0] = fanCurve->graph(0)->dataMainValue(qv_x.length() -1);
    rightLineY[1] = fanCurve->graph(0)->dataMainValue(qv_x.length() -1);

    fanCurve->graph(2)->setData(rightLineX, rightLineY);
    rePlot();
}

void FanCurvePlotWidget::detectRelease(QMouseEvent*)
{
    mousePressed = false;
    mouseMoving = false;
    mouseDragging = false;
    draggingPoint = false;
    coordText->setText("");
    rePlot();
}

void FanCurvePlotWidget::setXAxisRange(int min, int max, int maxMargin)
{
    fanCurve->xAxis->setRange(min, max + maxMargin);
    x_lower = min;
    x_upper = max;
}

void FanCurvePlotWidget::setYAxisRange(int min, int max, int maxMargin)
{
    fanCurve->yAxis->setRange(min, max + maxMargin);
    y_lower = min;
    y_upper = max;
}

void FanCurvePlotWidget::setXAxisLabel(QString& text)
{
    fanCurve->xAxis->setLabel(text);
}

void FanCurvePlotWidget::setYAxisLabel(QString& text)
{
    fanCurve->yAxis->setLabel(text);
}

void FanCurvePlotWidget::updateData()
{
    fanCurve->graph(0)->setData(qv_x, qv_y);
    rePlot();
    drawFillerLines();
}
