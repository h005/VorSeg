#include "face.h"

Face::Face()
{
    f = QPointF(-1,-1);
    edge = NULL;
    next = NULL;
}

Face::Face(QPointF f)
{
    this->f = f;
    edge = NULL;
    next = NULL;
}

Face::Face(QPointF f, Edge *edge)
{
    this->f = f;
    this->edge = edge;
    next = NULL;
}

Face* Face::insert(QPointF p)
{
    Face *q = this->next;
    while(q)
    {
        if(q->f == p)
            return q;
        q = q->next;
    }
    q = new Face(p);
    q->next = this->next;
    this->next = q;
    return q;
}
