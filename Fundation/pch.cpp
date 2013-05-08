#include "pch.h"

int i = CCNARGS(a, b, c);

class A
{
    void func();
    void func(int a);
    void func(int a, int b);
};

#define IMPLEMENT_CLASS A
void IMPLEMENT_FUNCTION(func)

}

void IMPLEMENT_FUNCTION(func, int a)

}

void IMPLEMENT_FUNCTION(func, int a, int b)

}
// get template args number.
template<typename T>
struct is_non_template_param
{
    enum { have_template_param =1 };
};
 
template<>
struct is_non_template_param<void>
{
    enum { have_template_param =0 };
};
 
template<typename T1,typename T2,typename T3,typename T4,typename T5>
struct template_params
{
    enum { count = is_non_template_param<T1>::have_template_param +
                  is_non_template_param<T2>::have_template_param +
                  is_non_template_param<T3>::have_template_param +
                  is_non_template_param<T4>::have_template_param +
                  is_non_template_param<T5>::have_template_param };
};

template<typename T1 = void,
         typename T2 = void,
        typename T3 = void,
        typename T4 = void,
        typename T5 = void>
struct Delegate
{
    enum{ ParamsCount = template_params<T1,T2,T3,T4,T5>::count };
    int GetParamsCount()
    {
        return ParamsCount;
    }
 
};
#include <functional>
#include <iostream>
using namespace std;

int add5(int i1, int i2, int i3, int i4, int i5)
{
    return i1 + i2 + i3 + i4 + i5;
}

int add10(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10)
{
    return i1 + i2 + i3 + i4 + i5 + i6 + i7 + i8 + i9 + i10;
};

void pchTest()
{
    Delegate<int,double,char> d1;
    Delegate<int,int*,char*,int> d2;
    Delegate<void> d3;
   
    cout <<"d1 params count =" << d1.GetParamsCount()<<endl;
    cout <<"d2 params count =" << d2.GetParamsCount()<<endl;
    cout <<"d3 params count =" << d3.GetParamsCount()<<endl;

    function<int (int, int, int, int, int)> f;
    f = bind(&add5, placeholders::_1, placeholders::_2, placeholders::_3, 
        placeholders::_4, placeholders::_5);

    int ret = f(1,2,3,4,5);
    cout << endl;
}
