#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <QPointF>
class DCEL;
class BeachLine;

class EventQueue
{
public:
    EventQueue();
    EventQueue(QPointF p,QPointF c,BeachLine *beachline);
    bool empty();
    QPointF getTopPoint();
    EventQueue* pop();
    void del(EventQueue *circleEvnet);
    QPointF getCenter();
    EventQueue* insert(QPointF *c1,QPointF *p1,BeachLine *bl);
    bool higher(EventQueue *q);


    EventQueue *next;
    QPointF p,c;
    BeachLine *beachline;

};

#endif // EVENTQUEUE_H
