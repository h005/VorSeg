#ifndef SITEQUEUE_H
#define SITEQUEUE_H

#include <QPointF>

class SiteQueue
{
public:
    SiteQueue();
    SiteQueue(QPointF p);
    SiteQueue* insert(QPointF p);
    SiteQueue* pop();
    QPointF getTopPoint();
    bool empty();
    bool higher(SiteQueue* q);
    QPointF p;
    SiteQueue *next;
};

#endif // SITEQUEUE_H
