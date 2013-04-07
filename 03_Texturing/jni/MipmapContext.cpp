#include "MipmapContext.h"

#include "Debug.h"
#include "SampleTools.h"
#include "Surface.h"
#include "OGLES2Tools.h"

using namespace std;

MipmapContext::MipmapContext()
{
    mProgram.id = 0;
}

MipmapContext::~MipmapContext()
{
}

bool MipmapContext::init()
{
    createProgram();
    loadIndicesAttribs();
    loadTextures();
    return true;
}

void MipmapContext::createProgram()
{
    const char* vss =
        "attribute  highp   vec2    attCoordPos;"
        "attribute  highp   vec2    attCoordTexMipmap;"
        "uniform    highp   float   uniScale;"
        "varying    mediump vec2    varCoordTexMipmap;"
        "void main(void)"
        "{"
        "    gl_Position = vec4(attCoordPos.x * uniScale, attCoordPos.y * uniScale, 0.0f, 1.0f);"
        "    varCoordTexMipmap = attCoordTexMipmap;"
        "}";
    const char* fss =
        "uniform    sampler2D       samMipmap;"
        "varying    mediump vec2    varCoordTexMipmap;"
        "void main (void)"
        "{"
        "    gl_FragColor = texture2D(samMipmap, varCoordTexMipmap);"
        "}";
    const char * attribs[] =
    {
            "attCoordPos",          // index = 0
            "attCoordTexMipmap",    // index = 1
    };
    mProgram.id = createProgramFromSource(
            vss,
            fss,
            attribs,
            (unsigned int *)&mProgram.attLocations.attCoordPos,
            2);

    mProgram.uniScale = glGetUniformLocation(mProgram.id, "uniScale");
    mProgram.samMipmap = glGetUniformLocation(mProgram.id, "samMipmap");
}

// Triangle indices index
//    2
//   / \
//  /   \
// 0-----1
void MipmapContext::loadIndicesAttribs()
{
    static float indices[] =
    {
            -1.0f,-1.0f,     0.0f, 0.0f,
             1.0f,-1.0f,     1.0f, 0.0f,
             0.0f, 1.0f,     0.5f, 1.0f,
    };
    mData.indices = indices;
}

void MipmapContext::loadTextures()
{
    if(PVRTTextureLoadFromPVR("texMipmap.pvr", &mData.texMipmap) != PVR_SUCCESS)
    {
        Debug::logd("PVRTTextureLoadFromPVR failed!!");
    }
    glBindTexture(GL_TEXTURE_2D, mData.texMipmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#define CLR_BKGND    0.0f, 0.0f, 0.0f, 1.0f
int MipmapContext::run()
{
    glClearColor(CLR_BKGND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawAnimation();

    return 0;
}
#define ANI_MAX_SCALE       2
#define ANIMATION_DELAY     4
void MipmapContext::drawAnimation()
{
    // Update mipmap scale factor
    static clock_t beginTime = clock();
    clock_t elapseTime = clock() - beginTime;
    float t = (float)(elapseTime % (CLOCKS_PER_SEC * ANIMATION_DELAY)) / CLOCKS_PER_SEC;
    float x = t / ANIMATION_DELAY;
    float scale = x * x * x * x * ANI_MAX_SCALE;

    // begin draw
    glUseProgram(mProgram.id);

    // Assign position coordinate attribute
    glEnableVertexAttribArray(mProgram.attLocations.attCoordPos);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordPos,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 4,
            mData.indices);
    // Assign mipmap texture coordinate attribute
    glEnableVertexAttribArray(mProgram.attLocations.attCoordTexMipmap);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordTexMipmap,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 4,
            mData.indices + 2);

    glUniform1f(mProgram.uniScale, scale);
    glBindTexture(GL_TEXTURE_2D, mData.texMipmap);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(mProgram.attLocations.attCoordTexMipmap);
    glDisableVertexAttribArray(mProgram.attLocations.attCoordPos);
}
