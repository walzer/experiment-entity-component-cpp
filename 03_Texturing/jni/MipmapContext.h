#ifndef __MIPMAP_CONTEXT_H__
#define __MIPMAP_CONTEXT_H__

#include "Context.h"
#include "Surface.h"

#include "GLES2/gl2.h"

class MipmapContext : public Context
{
public:
    MipmapContext();
    virtual ~MipmapContext();

    virtual bool init();
    virtual int run();

protected:
    void createProgram();
    void loadIndicesAttribs();
    void loadTextures();

    void drawAnimation();

    struct
    {
        GLuint id;
        struct
        {
            GLuint attCoordPos;
            GLuint attCoordTexMipmap;
        } attLocations;
        GLuint uniScale;
        GLuint samMipmap;
    } mProgram;
    struct
    {
        const float * indices;
        GLuint texMipmap;
    } mData;
};

#endif /* __MIPMAP_CONTEXT_H__ */
