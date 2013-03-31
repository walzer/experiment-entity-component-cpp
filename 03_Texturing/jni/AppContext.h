#ifndef __APP_CONTEXT_H__
#define __APP_CONTEXT_H__

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

    std::array<Coord, 4> mGratingTexCoords;
    static const unsigned kGratingW = 64;
    static const unsigned kGratingH = 64;
    std::shared_ptr<std::array<unsigned, kGratingW * kGratingH>> mTexGratingData;

    struct
    {
        GLuint id;
        struct
        {
            GLuint attCoordPos;
            GLuint attCoordTexImage;
            GLuint attCoordTexGrating;
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

#endif /* __APP_CONTEXT_H__ */
