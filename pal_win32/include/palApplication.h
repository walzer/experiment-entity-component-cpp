#ifndef __PAL__APPLICATION_H__
#define __PAL__APPLICATION_H__

#include "pal/Application.h"

namespace cc {;

namespace pal {;

class Application: public cc::Application {
public:
    Application();
    ~Application() override;

    int getErrorCode() override;
    void run();

    CC_DECL_COMPONENT_TYPE_INFO;

protected:
    bool _onInit(const IDom *args) override;
    void _onEnable() override;
    void _onDisable() override;
    void _onDone() override;
};

}   // namespace pal

}   // namespace cc

#endif  // __PAL__APPLICATION_H__
