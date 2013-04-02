#ifndef __MULTI_TEXTURE_CONTEXT_H__
#define __MULTI_TEXTURE_CONTEXT_H__

#include "Context.h"
#include "Surface.h"

#include "GLES2/gl2.h"

#include <array>
#include <memory>

struct Coord
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        struct
        {
            float u;
            float v;
        };
    };
};
class MultiTextureContext : public Context
{
public:
    MultiTextureContext();
    virtual ~MultiTextureContext();

    virtual bool init();
    virtual int run();

protected:
    void createProgram();
    void loadIndicesAttribs();
    void loadTextures();

    void drawAnimation();

    std::array<Coord, 4> mGratingTexCoords;

    struct
    {
        GLuint id;
        struct
        {
            GLuint attCoordPos;
            GLuint attCoordTexGrating;
            GLuint attCoordTexImage;
        } attLocations;
        GLuint samImage;
        GLuint samGrating;
    } mProgram;
    struct
    {
        const float * coordPos;
        const float * coordTexImage;
        GLuint texImage;
        GLuint texGrating;
    } mData;
};

#endif /* __MULTI_TEXTURE_CONTEXT_H__ */
