#ifndef DECL_H
#define DECL_H

#include "face.h"
#include "vertex.h"
#include "edge.h"


class DCEL
{
public:
    DCEL();
    Face *face;
    Vertex *vertex;
    Edge *edge;
    //DCEL *next;

    void print();
    void delEdgeCouple(Edge *e);

};

#endif // DECL_H
