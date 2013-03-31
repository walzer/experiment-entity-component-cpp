#include "AppContext.h"

#include "Debug.h"
#include "Runtime.h"
#include "SampleTools.h"

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
        "attribute  highp   vec2    attCoordTexImage;"
        "attribute  highp vec2    attCoordTexGrating;"
        "varying    mediump vec2    varCoordTexImage;"
        "varying    mediump vec2    varCoordTexGrating;"
        "void main(void)"
        "{"
        "    gl_Position = vec4(attCoordPos.x, attCoordPos.y, 0.0f, 1.0f);"
        "    varCoordTexImage = attCoordTexImage;"
        "    varCoordTexGrating = attCoordTexGrating;"
        "}";
    const char* fss =
        "varying    mediump vec2    varCoordTexImage;"
        "varying    mediump vec2    varCoordTexGrating;"
        "uniform    sampler2D       samImage;"
        "uniform    sampler2D       samGrating;"
        "void main (void)"
        "{"
        "    vec4 clrImg = texture2D(samImage, varCoordTexImage);"
        "    vec4 clrGrt = texture2D(samGrating, varCoordTexGrating);"
        "    gl_FragColor = (clrGrt.a < 0.1) ? clrImg : clrGrt;"
        "}";
    const char * attribs[] =
    {
            "attCoordPos",          // index = 0
            "attCoordTexImage",     // index = 1
            "attCoordTexGrating",   // index = 2
    };
    mProgram.id = createProgramFromSource(
            vss,
            fss,
            attribs,
            (unsigned int *)&mProgram.attLocations.attCoordPos,
            3);

    mProgram.samImage = glGetUniformLocation(mProgram.id, "samImage");
    mProgram.samGrating = glGetUniformLocation(mProgram.id, "samGrating");
}

// Rectangle indices index
// 2----3
// |\   |
// | \  |
// |  \ |
// |   \|
// 0----1
void AppContext::loadIndicesAttribs()
{
    static Coord coordPosArray[] =
    {
            {-1.0f,-1.0f},      { 1.0f,-1.0f},
            {-1.0f, 1.0f},      { 1.0f, 1.0f},
    };
    mData.coordPos = (const float *)coordPosArray;

    static Coord coordTexImageArray[] =
    {
            { 0.0f, 0.0f},      { 1.0f, 0.0f},
            { 0.0f, 1.0f},      { 1.0f, 1.0f},
    };
    mData.coordTexImage = (const float *)coordTexImageArray;
}

void AppContext::loadTextures()
{
    if(PVRTTextureLoadFromPVR("texPoint.pvr", &mData.texImage) != PVR_SUCCESS)
    {
        Debug::logd("PVRTTextureLoadFromPVR failed!!");
    }

    mTexGratingData.reset(new std::array<unsigned, kGratingW * kGratingH>());
#define CLR_MASK    0xffffffff
#define CLR_NONE    0x00000000
    static const unsigned gratingOneLineData[] =
    {
        CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,
        CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,CLR_NONE,

        CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,
        CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,

        CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,
        CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,

        CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,
        CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,CLR_MASK,
    };
    for (int i = 0; i < kGratingH; ++i)
    {
        for (int j = 0; j < kGratingW; ++j)
        {
            (*mTexGratingData)[i * kGratingW + j] = gratingOneLineData[j];
        }
    }
    glGenTextures(1, &mData.texGrating);
    glBindTexture(GL_TEXTURE_2D, mData.texGrating);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, kGratingW, kGratingH, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, mTexGratingData->data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#define CLR_BKGND    0.0f, 0.0f, 1.0f, 1.0f
int AppContext::run()
{
    glClearColor(CLR_BKGND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawAnimation();

    return 0;
}

void AppContext::drawAnimation()
{
    glUseProgram(mProgram.id);

    // Assign position coordinate attribute
    glEnableVertexAttribArray(mProgram.attLocations.attCoordPos);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordPos,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mData.coordPos);
    // Assign image texture coordinate attribute
    glEnableVertexAttribArray(mProgram.attLocations.attCoordTexImage);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordTexImage,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mData.coordTexImage);
//    // Assign grating texture coordinate attribute
    glEnableVertexAttribArray(mProgram.attLocations.attCoordTexGrating);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordTexGrating,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mData.coordTexImage);
//            mGratingTexCoords.data());

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(mProgram.samImage, 0);
    glBindTexture(GL_TEXTURE_2D, mData.texImage);

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(mProgram.samGrating, 1);
    glBindTexture(GL_TEXTURE_2D, mData.texGrating);

    static const unsigned char indices[] = { 0, 1, 2, 3};
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(mProgram.attLocations.attCoordTexGrating);
    glDisableVertexAttribArray(mProgram.attLocations.attCoordTexImage);
    glDisableVertexAttribArray(mProgram.attLocations.attCoordPos);
}
