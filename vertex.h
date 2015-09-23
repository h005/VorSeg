#ifndef VERTEX_H
#define VERTEX_H

#include <QPointF>

class Edge;

class Vertex
{
public:
    Vertex();
    Vertex(QPointF p);
    Vertex* insert(QPointF p);
    void setIncidentEdge(Edge *e);

    QPointF p;
    Edge *e;
    Vertex *next;
};

#endif // VERTEX_H
