#ifndef __APP_CONTEXT_H__
#define __APP_CONTEXT_H__

#include "Context.h"
#include "Surface.h"

#include "GLES2/gl2.h"

class AppContext : public Context
{
public:
    AppContext();
    virtual ~AppContext();

    virtual bool init();
    virtual int run();

    void onTouchEvent(Surface* sender, PointerData& args);
protected:
    struct
    {
        GLuint  id;
        GLuint  vs;
        GLuint  fs;
        struct
        {
            struct
            {
                int myPMVMatrix;
            } uniforms;
            struct
            {
                GLuint coord;
                GLuint color;
            } attributes;
        } Locations;

    } mProgram;

    struct
    {
        struct
        {
            const float * myPMVMatrix;
        } uniforms;
        struct
        {
            const float * myVertex;
        } attributes;
    }mData;
};

#endif /* __APP_CONTEXT_H__ */
