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
            GLuint attCoordPosNext;
            GLuint attCoordTex;
        } attLocations;
        GLuint samShark;
        GLuint uniStep;
        GLuint id;
    } mProgram;
    static const unsigned FRAME_COUNT = 8;
    struct
    {
        const float * coordPossFrames[FRAME_COUNT];
        const float * coordTexs;
        unsigned frameCount;
        const unsigned char * indices;
        unsigned indicesCount;
        GLuint texShark;
    } mData;
};

#endif /* __APP_CONTEXT_H__ */
