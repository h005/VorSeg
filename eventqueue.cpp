#include "eventqueue.h"

EventQueue::EventQueue()
{
    p = QPointF(-1,-1);
    c = QPointF(-1,-1);
    next = NULL;
    beachline = NULL;
}

EventQueue::EventQueue(QPointF c, QPointF p, BeachLine *beachline)
{
    this->p = p;
    this->c = c;
    this->beachline = beachline;
    next = NULL;
}

bool EventQueue::empty()
{
    return !this->next;
}

QPointF EventQueue::getTopPoint()
{
    return this->next->p;
}

EventQueue* EventQueue::pop()
{
    EventQueue* res;
    res = this->next;
    this->next = res->next;
    return res;
}

void EventQueue::del(EventQueue *circleEvnet)
{
    EventQueue *tmp,*p;
    p = this;
    while(p->next && p->next!=circleEvnet)
        p = p->next;
    if(p->next)
    {
        tmp = p->next;
        p->next = tmp->next;
        delete tmp;
    }
}

QPointF EventQueue::getCenter()
{
    return c;
}

EventQueue* EventQueue::insert(QPointF *c1, QPointF *p1, BeachLine *bl)
{
    EventQueue *res = new EventQueue(*c1,*p1,bl);
    EventQueue *p2 = this;
    while(p2->next && p2->next->higher(res))
        p2 = p2->next;
    res->next = p2->next;
    p2->next = res;
    return res;
}

bool EventQueue::higher(EventQueue *q)
{
    return p.y()==q->p.y()?
                p.x()<q->p.x():
                p.y()<q->p.y();
}
