#include "beachline.h"
#include <math.h>

BeachLine::BeachLine()
{
    p = QPointF(-1,-1);
    next = NULL;
    circleEvent = NULL;
    prev = NULL;
}

BeachLine::BeachLine(QPointF p)
{
    this->p = p;
    next = NULL;
    circleEvent = NULL;
    prev = NULL;
}

bool BeachLine::empty()
{
    return !this->next;
}

BeachLine* BeachLine::insert(QPointF p)
{
    BeachLine *tmp = new BeachLine(p);
    if(this->next)
    {
        this->next->prev = tmp;
        tmp->prev = this;
        tmp->next = this->next;
        this->next = tmp;
    }
    else
    {
        this->next = tmp;
        tmp->prev = this;
    }
    //this->next = tmp;
    return tmp;
}

bool BeachLine::higher(QPointF q)
{
    return p.y()==q.y()
            ?p.x()<q.x()
            :p.y()<q.y();
}

QPointF* BeachLine::checkCircleL(BeachLine *b1, BeachLine *b2, BeachLine *b3)
{
    QPointF *center;
    //check...
    if(b1 == this)
    {
        //center = NULL;
        return NULL;
    }
    if(b3 == NULL)
    {
        //center = NULL;
        return NULL;
    }
    QPointF v1 = b2->p-b3->p;
    QPointF v2 = b1->p-b3->p;
    double tmpres = v1.x()*v2.y()-v2.x()*v1.y();

    //if ok then
//    if(tmpres)
    /*
     * 检测b1,b2,b3的共圆，
     * 主要是检测beachLine b2所对应的抛物线是否会消失，
     * 虽然任意给定三条beachLine 只要tmpres非零，是一定
     * 可以得到圆心的，但是未必是与b2对应
     * 所以需要加入这个条件来限制
     */
    if(tmpres<0)
    {
        center = getCircleCenter(b1,b2,b3);
        return center;
    }
    else
    {
        center = NULL;
        return center;
    }

}

QPointF* BeachLine::checkCircleR(BeachLine *b1, BeachLine *b2, BeachLine *b3)
{
    QPointF *center;
    if(b1 == this)
        return NULL;
    if(b3 == NULL)
        return NULL;
    QPointF v1 = b3->p-b1->p;
    QPointF v2 = b2->p-b1->p;
    double tmpres = v1.x()*v2.y()-v2.x()*v1.y();

//    if(tmpres)
    if(tmpres<0)
    {
        center = getCircleCenter(b1,b2,b3);
        return center;
    }
    else
    {
        center = NULL;
        return center;
    }

}

QPointF* BeachLine::getCircleCenter(BeachLine *b1, BeachLine *b2, BeachLine *b3)
{
    QPointF *center;
    double x1 = b1->p.x();
    double x2 = b2->p.x();
    double x3 = b3->p.x();
    double y1 = b1->p.y();
    double y2 = b2->p.y();
    double y3 = b3->p.y();
    double x4,y4;

    x4 = -((x1*x1+y1*y1)*(y3-y2)-(x2*x2+y2*y2)*(y3-y1)+(x3*x3+y3*y3)*(y2-y1))
        /(x1*(y2-y3)-x2*(y1-y3)+x3*(y1-y2))/2.0;
    y4 = -((x1*x1+y1*y1)*(x2-x3)-(x2*x2+y2*y2)*(x1-x3)+(x3*x3+y3*y3)*(x1-x2))
        /(x1*(y2-y3)-x2*(y1-y3)+x3*(y1-y2))/2.0;
    //x4=(pow(X2,2)*Y1-pow(X3,2)*Y1-pow(X1,2)*Y2+pow(X3,2)*Y2-pow(Y1,2)*Y2+pow(Y2,2)*Y1+pow(X1,2)*Y3-pow(X2,2)*Y3+pow(Y1,2)*Y3-pow(Y2,2)*Y3-pow(Y3,2)*Y1+pow(Y3,2)*Y2)/(2*(X2*Y1-X3*Y1-X1*Y2+X3*Y2+X1*Y3-X2*Y3));
    //y4=-(-pow(X1,2)*X2+pow(X2,2)*X1+pow(X1,2)*X3-pow(X2,2)*X3-pow(X3,2)*X1+pow(X3,2)*X2-pow(Y1,2)*X2+pow(Y1,2)*X3+pow(Y2,2)*X1-pow(Y2,2)*X3-pow(Y3,2)*X1+pow(X3,2)*X2)/(2*(X2*Y1-X3*Y1-X1*Y2+X3*Y2+X1*Y3-X2*Y3));
    center = new QPointF(x4,y4);
    return center;
}

BeachLine *BeachLine::isDegenerateCase(QPointF p)
{
    BeachLine *q = this;
    while(q->next && q->next->p.y() == p.y())
        q = q->next;
    // DegenerateCase
    if(!q->next)
        return q;
    else
        return NULL;
}

void BeachLine::del(BeachLine *p)
{
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
}

BeachLine* BeachLine::search(QPointF p)
{
    BeachLine *q = this->next;
    while(q && q->next)
    {
        double x1 = q->p.x(),x2 = q->next->p.x();
        double y1 = q->p.y(),y2 = q->next->p.y();
        double y0 = p.y();
        /*
        double p1 = p.y()-q->p.y();
        double p2 = p.y()-q->next->p.y();
        double a = p2-p1;
        double b = 2.0*p1*x2-2.0*p2*x1;
        double c = p2*x1*x1-p1*x2*x2-2.0*p1*p2*q->next->p.y()+
                2.0*p1*p2*q->p.y()+p1*p2*p2-p1*p1*p2;
        */
        double p1 = y0 - y1;
        double p2 = y0 - y2;
        double a = p2-p1;
        double b = 2*p1*x2-2*p2*x1;
        double c = p2*x1*x1-p1*x2*x2+2*p1*p2*y2-2*p1*p2*y1+p1*p2*p2-p2*p1*p1;
        if(a)
        {
            x1 = (-b-sqrt(b*b-4.0*a*c))/2.0/a;
            x2 = (-b+sqrt(b*b-4.0*a*c))/2.0/a;
            if(y1<=y2)
                x1 = x1>x2?x2:x1;
            else
                x1 = x1>x2?x1:x2;
        }
        else
            x1 = -c/b;
        //x1 = x1<x2?x1:x2;
        if(x1<p.x())
            q = q->next;
        else
            break;
        /*
        x2 = (-b+sqrt(b*b-4.0*a*c))/2.0/a;
        if(x1>=q->p.x() && x1<=q->next->p.x())
        {
            if(x1<p.x())
                q = q->next;
            else
                break;
        }
        else
        {
            if(x2<p.x())
                q = q->next;
            else
                break;
        }
         */
    }
    return q;
}
