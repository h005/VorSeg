#include "uf.h"
#include <string.h>

UF::UF()
{

}

UF::UF(int num)
{
    id = new int[num];
    sz = new int[num];
    memset(sz,0,sizeof(int)*num);
    for(int i=0;i<num;i++)
        id[i] = i;
}

void UF::uni(int p, int q)
{
    int fp = find(p);
    int fq = find(q);
    if(fp == fq)
        return;
    if(sz[fp] > sz[fq])
    {
        id[fq] = fp;
        sz[fp] += sz[fq];
    }
    else
    {
        id[fp] = fq;
        sz[fq] += sz[fp];
    }
}

int UF::find(int p)
{
    while(p != id[id[p]])
    {
        id[p] = id[id[p]];
        p = id[p];
    }
    return p;
}

