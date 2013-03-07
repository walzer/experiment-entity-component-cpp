#ifndef HELLOCONTEXT_H_
#define HELLOCONTEXT_H_

#include "Context.h"

class HelloContext : public Context
{
public:
    HelloContext();
    virtual ~HelloContext();

    virtual int run();
};

#endif /* HELLOCONTEXT_H_ */
