#include "AppContext.h"

#include "Debug.h"
#include "SampleTools.h"
#include "Surface.h"
#include "OGLES2Tools.h"

using namespace std;

AppContext::AppContext()
{
    mProgram.id = 0;
}

AppContext::~AppContext()
{
}

bool AppContext::init()
{
    createProgram();
    loadIndicesAttribs();
    loadTextures();
    return true;
}

void AppContext::createProgram()
{
    const char* vss =
        "attribute  highp   vec2    attCoordPos;"
        "attribute  highp   vec2    attCoordTex;"
        "varying    mediump vec2    varCoordTex;"
        "void main(void)"
        "{"
        "    gl_Position = vec4(attCoordPos, 0, 1);"
        "    varCoordTex = attCoordTex;"
        "}";
    const char* fss =
        "uniform    sampler2D       samShark;"
        "varying    mediump vec2    varCoordTex;"
        "void main (void)"
        "{"
        "    gl_FragColor = texture2D(samShark, varCoordTex);"
        "}";
    const char * attribs[] =
    {
            "attCoordPos",          // index = 0
            "attCoordTex",          // index = 1
    };
    mProgram.id = createProgramFromSource(
            vss,
            fss,
            attribs,
            (unsigned int *)&mProgram.attLocations.attCoordPos,
            2);

    Debug::logd("Program(%d)(%d, %d)",
            mProgram.id,
            mProgram.attLocations.attCoordPos,
            mProgram.attLocations.attCoordTex);
    mProgram.samShark = glGetUniformLocation(mProgram.id, "samShark");
}

void AppContext::loadIndicesAttribs()
{
    static float indices[] =
    {
            -1.0f,-1.0f,     0.0f, 0.0f,
             1.0f,-1.0f,     1.0f, 0.0f,
             0.0f, 1.0f,     0.5f, 1.0f,
    };
    mData.indices = indices;
}

void AppContext::loadTextures()
{
    if(PVRTTextureLoadFromPVR("texShark.pvr", &mData.texShark) != PVR_SUCCESS)
    {
        Debug::logd("PVRTTextureLoadFromPVR failed!!");
    }
}

#define CLR_BKGND    1.0f, 0.0f, 0.0f, 1.0f
int AppContext::run()
{
    glClearColor(CLR_BKGND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawAnimation();

    return 0;
}
void AppContext::drawAnimation()
{
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
    glEnableVertexAttribArray(mProgram.attLocations.attCoordTex);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordTex,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 4,
            mData.indices + 2);

    glBindTexture(GL_TEXTURE_2D, mData.texShark);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(mProgram.attLocations.attCoordTex);
    glDisableVertexAttribArray(mProgram.attLocations.attCoordPos);

}
