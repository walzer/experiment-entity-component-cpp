#ifndef HELLOCONTEXT2_H_
#define HELLOCONTEXT2_H_

#include "HelloContext.h"

class HelloContext2: public HelloContext
{
public:
    virtual bool init();
    virtual int run();

public:
    const float * mCoordsArray;
    const float * mColorsArray;
};

#endif /* HELLOCONTEXT2_H_ */
