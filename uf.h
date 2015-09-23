#ifndef UF_H
#define UF_H


class UF
{
public:
    UF();
    UF(int num);
    void uni(int p,int q);
    int find(int p);

private:
    int *id;
    int *sz;
};

#endif // UF_H
