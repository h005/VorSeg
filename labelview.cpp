#include "labelview.h"
#include <QDebug>

LabelView::LabelView(QWidget *parent) :
    QLabel(parent)
{
    Flag_Points = false;
    Flag_Voronoi = false;
    Flag_linePoints = false;
    linePoints = new QPointF[NUM_LinePoints];
    countlp = 0;
    uf = NULL;
}

LabelView::~LabelView()
{

}

void LabelView::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    Flag_linePoints = true;
}

void LabelView::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);
    if(Flag_linePoints)
        linePoints[countlp++] = event->pos();
    update();
}

void LabelView::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    areaVoronoi();
    countlp = 0;
    update();
}

void LabelView::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter *painter = new QPainter(this);

    if(Flag_Points)
    {
        painter->setPen(QPen(Qt::red,3));
        for(int i = 0;i<points.size();i++)
            painter->drawPoint(points[i]);
    }
    if(Flag_Voronoi)
    {
        painter->setPen(QPen(Qt::green,2));
        Edge *e = dcel->edge->next;
        while(e)
        {
            painter->drawLine(e->origin->p,e->twin->origin->p);
            e = e->next;
        }
    }
    if(Flag_linePoints)
    {
        painter->setPen(QPen(Qt::blue,1));
        for(int i=0;i<countlp-1;i++)
            painter->drawLine(linePoints[i],linePoints[i+1]);
    }
}

void LabelView::setPoints(Mat &grayImage)
{
    int thresh = 150;
    Mat dst = Mat::zeros(grayImage.size(),CV_32FC1);

    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    cornerHarris(grayImage,dst,blockSize,apertureSize,k);

    normalize(dst,dst,0,255,NORM_MINMAX,CV_32FC1,Mat());

    for(int j=0;j<dst.cols;j++)
        for(int i=0;i<dst.rows;i++)
            if((int)dst.at<float>(i,j) > thresh)
                points.push_back(QPointF(j,i));

    Flag_Points = true;
    Flag_Voronoi = false;
    uf = new UF(points.size());
    update();
}

void LabelView::voronoi()
{
    sitequeue = new SiteQueue();
    eventqueue = new EventQueue();
    beachline = new BeachLine();
    dcel = new DCEL();

    for(int i=0;i<points.size();i++)
        sitequeue->insert(points[i]);

    while(!sitequeue->empty())
    {
        if(eventqueue->empty())
        {
            SiteQueue *tmp = sitequeue->pop();
            handleSiteEvent(tmp,eventqueue,beachline,dcel);
        }
        else
        {
            QPointF ps = sitequeue->getTopPoint();
            QPointF pc = eventqueue->getTopPoint();
            bool tmpbool = ps.y()==pc.y()?
                        ps.x()<pc.x():
                        ps.y()<pc.y();
            if(tmpbool)
            {
                SiteQueue *tmp = sitequeue->pop();
                handleSiteEvent(tmp,eventqueue,beachline,dcel);
            }
            else
            {
                EventQueue *tmp = eventqueue->pop();
                handleCircleEvent(sitequeue,eventqueue,beachline,dcel,tmp->beachline);
            }
        }
    }
    while(!eventqueue->empty())
    {
        EventQueue *tmp = eventqueue->pop();
        handleCircleEvent(sitequeue,eventqueue,beachline,dcel,tmp->beachline);
    }

    formatDCEL();

    Flag_Voronoi = true;
    update();}

void LabelView::handleSiteEvent(SiteQueue *tmp,
                                EventQueue *eventqueue,
                                BeachLine *beachline,
                                DCEL *dcel)
{
    if(beachline->empty())
    {
        beachline->insert(tmp->p);
        dcel->face->insert(tmp->p);
        return;
    }
    // 处理degenerateCase
    BeachLine *upon = beachline->isDegenerateCase(tmp->p);
    if(upon)
    {
        BeachLine *b1 = upon->insert(tmp->p);
        Face *f1 = dcel->face->insert(b1->p);
        Face *f2 = dcel->face->insert(upon->p);
        Edge *e1 = dcel->edge->insert(f1);
        Edge *e2 = dcel->edge->insert(f2);
        e1->twin = e2;
        e2->twin = e1;
    }
    else
    {
        upon = beachline->search(tmp->p);
        if(upon->circleEvent)
        {
            eventqueue->del(upon->circleEvent);
            upon->circleEvent = NULL;
        }
        BeachLine *b1 = upon->insert(tmp->p);
        BeachLine *b2 = b1->insert(upon->p);
        //if exist the face then return the pointer
        //else insert the new face and return
        Face *f1 = dcel->face->insert(b1->p);
        Face *f2 = dcel->face->insert(b2->p);
        Edge *e1 = dcel->edge->insert(f1);
        Edge *e2 = dcel->edge->insert(f2);
        e1->twin = e2;
        e2->twin = e1;


        QPointF *c1 = beachline->checkCircleL(upon->prev,upon,b1);
        if(c1)
        {
            QPointF *p1 = getLowestPoint(c1,b1);
            EventQueue *ce1 = eventqueue->insert(c1,p1,upon);
            upon->circleEvent = ce1;
        }
        QPointF *c2 = beachline->checkCircleR(b1,b2,b2->next);
        if(c2)
        {
            QPointF *p2 = getLowestPoint(c2,b2);
            EventQueue *ce2 = eventqueue->insert(c2,p2,b2);
            b2->circleEvent = ce2;
        }
    }
}

void LabelView::handleCircleEvent(SiteQueue *sitequeue,
                                  EventQueue *eventqueue,
                                  BeachLine *beachline,
                                  DCEL *dcel,
                                  BeachLine *bl)
{
    eventqueue->del(bl->prev->circleEvent);
    eventqueue->del(bl->next->circleEvent);
    Vertex *v = dcel->vertex->insert(bl->circleEvent->getCenter());
    Face *f1 = dcel->face->insert(bl->prev->p);
    Face *f2 = dcel->face->insert(bl->next->p);
    Edge *e1 = dcel->edge->insert(f1);
    Edge *e2 = dcel->edge->insert(f2);
    e1->twin = e2;
    e2->twin = e1;
    e2->setOrigin(v);
    dcel->edge->setOrigin(f1->f,bl->p,v);
    dcel->edge->setOrigin(bl->p,f2->f,v);
    v->setIncidentEdge(e2);

    BeachLine *b1 = bl->prev;
    BeachLine *b2 = bl->next;
    QPointF *c1 = beachline->checkCircleL(b1->prev,b1,b2);
    if(c1)
    {
        QPointF *p1 = getLowestPoint(c1,b1);
        EventQueue *ce1 = eventqueue->insert(c1,p1,b1);
        b1->circleEvent = ce1;
    }

    b1 = bl->next;
    b2 = bl->prev;
    QPointF *c2 = beachline->checkCircleR(b2,b1,b1->next);
    if(c2)
    {
        QPointF *p2 = getLowestPoint(c2,b1);
        EventQueue *ce2 = eventqueue->insert(c2,p2,b1);
        b1->circleEvent = ce2;
    }

    beachline->del(bl);

}

QPointF *LabelView::getLowestPoint(QPointF *c, BeachLine *bl)
{
    QPointF *p1;
    double rr = c->dotProduct(*c - bl->p, *c - bl->p);
    double y = c->y() + sqrt(rr);
    p1 = new QPointF(c->x(),y);
    return p1;
}

void LabelView::setUF()
{
    if(countlp == 0)
        return ;
    int lastId = 0;
    int tempId = 0;
    double dis = 0;
    double min = 1e10;
    // get lastId
    for(int i = 0;i<points.size();i++)
    {
        dis = QPointF::dotProduct(linePoints[0] - points[i],linePoints[0] - points[i]);
        if(dis < min)
        {
            min = dis;
            lastId = i;
        }
    }

    for(int i=1;i<countlp;i++)
    {
        min = 1e10;
        for(int j=0;j<points.size();j++)
        {
            dis = QPointF::dotProduct(linePoints[i] - points[j],linePoints[i] - points[j]);
            if(dis < min)
            {
                min = dis;
                tempId = j;
            }
        }
        uf->uni(lastId,tempId);
        lastId = tempId;
    }
}

void LabelView::formatDCEL()
{
    Edge *e = dcel->edge->next;
    while(e)
    {
        if(e->origin)
        {
            if(!e->twin->origin)
            {
                QPointF mid = (e->f->f+e->twin->f->f)/2;
                e->getOrigin(mid,e->origin->p,Inf);
            }
        }
        else
        {
            if(e->twin->origin)
            {
                QPointF mid = (e->f->f+e->twin->f->f)/2;
                e->twin->getOrigin(mid,e->twin->origin->p,Inf);
            }
            else
            {
                QPointF mid = (e->f->f+e->twin->f->f)/2;
                e->getOrigin(mid,Inf);
            }
        }
        e = e->next;
    }
}

void LabelView::areaVoronoi()
{
    setUF();
    Edge *e = dcel->edge;
    while(e->next)
    {
        int id1 = getPointsId(e->next->f->f);
        int id2 = getPointsId(e->next->twin->f->f);
        if(uf->find(id1) == uf->find(id2))
            dcel->delEdgeCouple(e->next);
        e = e->next;
    }
}

int LabelView::getPointsId(QPointF p)
{
    for(int i=0;i<points.size();i++)
        if(points[i] == p)
            return i;
    return 0;
}

