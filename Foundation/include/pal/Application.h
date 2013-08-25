#ifndef __FOUNDATION__APPLICATION_H__
#define __FOUNDATION__APPLICATION_H__

#include "IComponent.h"

namespace cc {;

class Application: public IComponent {
public:
    virtual ~Application();
    virtual int getErrorCode() = 0;

    static Application *getInstance() {
        return s_instance;
    }

protected:
    Application();

private:
    static Application *s_instance;
};

extern void initApplication(Application &app);

}   // namespace cc

#endif  // __FOUNDATION__APPLICATION_H__
