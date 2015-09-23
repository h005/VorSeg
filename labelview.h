#ifndef LABELVIEW_H
#define LABELVIEW_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <iostream>
#include <math.h>

#include <opencv.hpp>
#include <vector>
#include <QPointF>
#include "sitequeue.h"
#include "eventqueue.h"
#include "beachline.h"
#include "edge.h"
#include "dcel.h"
#include "face.h"
#include "vertex.h"
#include "uf.h"

#define Inf 10000000
#define NUM_LinePoints 2000

using namespace std;
using namespace cv;


class LabelView : public QLabel
{
    Q_OBJECT
public:
    explicit LabelView(QWidget *parent = 0);
    ~LabelView();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    void setPoints(Mat &grayImage);
    void voronoi();


private:

    SiteQueue *sitequeue;
    EventQueue *eventqueue;
    BeachLine *beachline;
    DCEL *dcel;
    UF *uf;

    vector<QPointF> points;
    QPointF *linePoints;
    int countlp;

    bool Flag_Points;
    bool Flag_Voronoi;
    bool Flag_linePoints;

private:

    void handleSiteEvent(SiteQueue *tmp,
                              EventQueue *eventqueue,
                              BeachLine *beachline,
                              DCEL *dcel
                              );

    void handleCircleEvent(SiteQueue *sitequeue,
                           EventQueue *eventqueue,
                           BeachLine *beachline,
                           DCEL *dcel,
                           BeachLine *bl
                           );
    QPointF* getLowestPoint(QPointF *c,BeachLine *bl);

    void setUF();

    void formatDCEL();

    void areaVoronoi();

    int getPointsId(QPointF p);

};

#endif // LABELVIEW_H
