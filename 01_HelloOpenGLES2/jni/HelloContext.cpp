#include "HelloContext.h"

#include <GLES2/gl2.h>

HelloContext::HelloContext()
{
}

HelloContext::~HelloContext()
{
}

int HelloContext::run()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return 0;
}
