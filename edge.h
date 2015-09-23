#ifndef EDGE_H
#define EDGE_H

#include <QPointF>
#include "face.h"
#include "vertex.h"
//class Face;
//class Vertex;

class Edge
{
public:
    Edge();
    Edge(Face *f);
    Edge* insert(Face *f);
    void setOrigin(QPointF p1,QPointF p2,QPointF p3);
    void setOrigin(QPointF p);
    void setOrigin(QPointF p1,QPointF p2,Vertex *p3);
    void setOrigin(Vertex *v);
    void print();
    QPointF getOrigin(QPointF mid,QPointF e1,double bound);
    void getOrigin(QPointF mid,double bound);

    Vertex *origin;
    Face *f;
    Edge *twin,*next,*prev;


};

#endif // EDGE_H
