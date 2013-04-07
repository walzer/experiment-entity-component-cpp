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

protected:
    void createProgram();
    void loadIndicesAttribs();
    void loadTextures();

    void drawAnimation();

    struct
    {
        struct
        {
            GLuint attCoordPos;
            GLuint attCoordTex;
        } attLocations;
        GLuint samShark;
        GLuint id;
    } mProgram;
    struct
    {
        const float * indices;
        GLuint texShark;
    } mData;
};

#endif /* __APP_CONTEXT_H__ */
