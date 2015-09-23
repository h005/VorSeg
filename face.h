#ifndef FACE_H
#define FACE_H

#include <QPointF>
class Edge;

class Face
{
public:
    Face();
    Face(QPointF f);
    Face(QPointF f,Edge *edge);
    Face* insert(QPointF p);

    QPointF f;
    Edge *edge;
    Face *next;
};

#endif // FACE_H
