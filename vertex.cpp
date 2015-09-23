#include "vertex.h"

Vertex::Vertex()
{
    p = QPointF(-1,-1);
    e = NULL;
    next = NULL;
}

Vertex::Vertex(QPointF p)
{
    this->p = p;
    e = NULL;
    next = NULL;
}

Vertex* Vertex::insert(QPointF p)
{
    Vertex *q = new Vertex(p);
    q->next = this->next;
    this->next = q;
    return q;
}

void Vertex::setIncidentEdge(Edge *e)
{
    this->e = e;
}
