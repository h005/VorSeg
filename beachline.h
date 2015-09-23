#ifndef BEACHLINE_H
#define BEACHLINE_H

#include <QPointF>

class EventQueue;

class BeachLine
{
public:
    BeachLine();
    BeachLine(QPointF p);
    bool empty();
    BeachLine* insert(QPointF p);
    bool higher(QPointF p);
    BeachLine* search(QPointF p);
    QPointF* checkCircleL(BeachLine *b1,BeachLine *b2,BeachLine *b3);
    QPointF* checkCircleR(BeachLine *b1,BeachLine *b2,BeachLine *b3);
    void del(BeachLine* p);
    QPointF* getCircleCenter(BeachLine *b1,BeachLine *b2,BeachLine *b3);
    BeachLine* isDegenerateCase(QPointF p);

    QPointF p;
    BeachLine *prev;
    BeachLine *next;
    EventQueue *circleEvent;

};

#endif // BEACHLINE_H
