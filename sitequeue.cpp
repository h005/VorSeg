#include "sitequeue.h"

SiteQueue::SiteQueue()
{
    p = QPointF(-1,-1);
    next = NULL;
}

SiteQueue::SiteQueue(QPointF p)
{
    this->p = p;
    next = NULL;
}

SiteQueue* SiteQueue::insert(QPointF p)
{
    SiteQueue *q = new SiteQueue(p);
    SiteQueue *p1 = this;
    while(p1->next && p1->next->higher(q))
        p1 = p1->next;
    q->next = p1->next;
    p1->next = q;
    return q;
}

bool SiteQueue::higher(SiteQueue *q)
{
    return    p.y()==q->p.y()
               ?p.x()<q->p.x()
               :p.y()<q->p.y();
}

bool SiteQueue::empty()
{
    return !this->next;
}

SiteQueue* SiteQueue::pop()
{
    SiteQueue *res = this->next;
    this->next = res->next;
    return res;
}

QPointF SiteQueue::getTopPoint()
{
    return this->next->p;
}
