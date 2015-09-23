#include "edge.h"
#include <stdio.h>

Edge::Edge()
{
    origin = NULL;
    f = NULL;
    twin = next = prev = NULL;
}

Edge::Edge(Face *f)
{
    origin = NULL;
    this->f = f;
    twin = next = prev = NULL;
}

Edge* Edge::insert(Face *f)
{
    Edge *p = new Edge(f);
    p->next = this->next;
    this->next = p;
    return p;
}

void Edge::setOrigin(QPointF p1, QPointF p2, QPointF p3)
{
    Edge *p,*q;
    p = this->next;
    while(p)
    {
        if(p->f->f==p1 && p->twin->f->f==p2)
        {
            p->origin = new Vertex(p3);
            return;
        }
        p = p->next;
    }
    printf("error no such edge (%lf,%lf) (%lf,%lf)\n",p1.x(),p1.y(),p2.x(),p2.y());
}

void Edge::setOrigin(QPointF p1, QPointF p2, Vertex *p3)
{
    Edge *p,*q;
    p = this->next;
    while(p)
    {
        if(p->f->f==p1 && p->twin->f->f==p2)
        {
            p->origin = p3;
            return;
        }
        p = p->next;
    }
    printf("error no such edge (%lf,%lf) (%lf,%lf)\n",p1.x(),p1.y(),p2.x(),p2.y());
}

void Edge::setOrigin(QPointF p)
{
     origin = new Vertex(p);
}

void Edge::setOrigin(Vertex *v)
{
    origin = v;
}

void Edge::print()
{
    Edge *p = this->next;
    while(p)
    {
        if(p->origin && p->twin->origin)
        {
            printf("%lf %lf %lf %lf\n",p->origin->p.x(),p->origin->p.y(),
                   p->twin->origin->p.x(),p->twin->origin->p.y());
            p = p->next;
        }
    }

}

QPointF Edge::getOrigin(QPointF mid,QPointF e1,double bound)
{
    if(mid == e1)
    {
        QPointF e0;
        if(this->f->f.y()==this->twin->f->f.y())
        {
            e0 = QPointF(mid.x(),mid.y()+2);
        }
        else
        {
            double k = (this->f->f.x()-this->twin->f->f.x())/
                    (this->twin->f->f.y()-this->f->f.y());

            e0 = QPointF(2+mid.x(),k*2+mid.y());
        }
        QPointF e = QPointF(e0.x()-bound*(e1.x()-e0.x()),e0.y()-bound*(e1.y()-e0.y()));
        QPointF p1 = e-e0;
        QPointF p2 = this->f->f-mid;
        if(p1.x()*p2.y()-p1.y()*p2.x()>0)
            e = -e;
        this->twin->origin = new Vertex(e);
        return e;
    }
    else
    {
        QPointF *e2 = new QPointF(mid.x()-bound*(e1.x()-mid.x()),mid.y()-bound*(e1.y()-mid.y()));
        QPointF p1 = *e2-mid;
        QPointF p2 = this->f->f-mid;
        if((p1.x()*p2.y()-p1.y()*p2.x())>0)
            *e2 = -*e2;
        this->twin->origin = new Vertex(*e2);
        return *e2;
    }
}

void Edge::getOrigin(QPointF mid, double bound)
{
    if(this->f->f.y()==this->twin->f->f.y())
    {
        QPointF e1 = QPointF(mid.x(),bound);
        QPointF e2 = QPointF(mid.x(),-bound);
        this->origin = new Vertex(e2);
        this->twin->origin = new Vertex(e1);
    }
    else
    {
        double k = (this->f->f.x()-this->twin->f->f.x())/
                (this->twin->f->f.y()-this->f->f.y());
        QPointF e1 = QPointF(-bound,k*(-bound-mid.x())+mid.y());
        QPointF e2 = QPointF(bound,k*(bound-mid.x())+mid.y());
        this->origin = new Vertex(e2);
        this->twin->origin = new Vertex(e1);
    }
}

