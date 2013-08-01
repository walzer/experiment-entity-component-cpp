#ifndef __FOUNDATION_TEST__FOO_COMPONENT_H__
#define __FOUNDATION_TEST__FOO_COMPONENT_H__

#include "IComponent.h"

class FooComponent: public ::cc::IComponent {
public:
    FooComponent();

    int calledOnInit;
    int calledOnEnable;

    CC_DECL_COMPONENT_TYPE_INFO;
protected:
    virtual bool _onInit(const ::cc::IDom *);
    virtual void _onEnable();
    virtual void _onDisable();
    virtual void _onDone();
};

#endif  // __FOUNDATION_TEST__FOO_COMPONENT_H__
