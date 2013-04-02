#include "MultiTextureContext.h"

#include "Debug.h"
#include "Runtime.h"
#include "SampleTools.h"
#include "Surface.h"
#include "OGLES2Tools.h"

using namespace std;

MultiTextureContext::MultiTextureContext()
{
    mProgram.id = 0;
}

MultiTextureContext::~MultiTextureContext()
{
}

bool MultiTextureContext::init()
{
    createProgram();
    loadIndicesAttribs();
    loadTextures();
    return true;
}

void MultiTextureContext::createProgram()
{
    const char* vss =
        "attribute  highp   vec2    attCoordPos;"
        "attribute  highp   vec2    attCoordTexGrating;"
        "attribute  highp   vec2    attCoordTexImage;"
        "varying    mediump vec2    varCoordTexGrating;"
        "varying    mediump vec2    varCoordTexImage;"
        "void main(void)"
        "{"
        "    gl_Position = vec4(attCoordPos.x, attCoordPos.y, 0.0f, 1.0f);"
        "    varCoordTexGrating = attCoordTexGrating;"
        "    varCoordTexImage = attCoordTexImage;"
        "}";
    const char* fss =
        "uniform    sampler2D       samGrating;"
        "uniform    sampler2D       samImage;"
        "varying    mediump vec2    varCoordTexGrating;"
        "varying    mediump vec2    varCoordTexImage;"
        "void main (void)"
        "{"
        "    vec4 clrGrt = texture2D(samGrating, varCoordTexGrating);"
        "    vec4 clrImg = texture2D(samImage, varCoordTexImage);"
        "    gl_FragColor = (clrGrt.a < 0.1) ? clrImg : clrGrt;"
        "}";
    const char * attribs[] =
    {
            "attCoordPos",          // index = 0
            "attCoordTexGrating",   // index = 1
            "attCoordTexImage",     // index = 2
    };
    mProgram.id = createProgramFromSource(
            vss,
            fss,
            attribs,
            (unsigned int *)&mProgram.attLocations.attCoordPos,
            3);

    mProgram.samGrating = glGetUniformLocation(mProgram.id, "samGrating");
    mProgram.samImage = glGetUniformLocation(mProgram.id, "samImage");
}

// Rectangle indices index
// 2----3
// |\   |
// | \  |
// |  \ |
// |   \|
// 0----1
void MultiTextureContext::loadIndicesAttribs()
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

#define GRATINGW    8
#define GRATINGH    1
#define GRATRATE    4
#define GRAT_GPS    1

void MultiTextureContext::loadTextures()
{
    if(PVRTTextureLoadFromPVR("texImage.pvr", &mData.texImage) != PVR_SUCCESS)
    {
        Debug::logd("PVRTTextureLoadFromPVR failed!!");
    }

#define CLR_MASK    0xff000000
#define CLR_NONE    0x00000000
    unsigned * data = new unsigned[GRATINGW * GRATINGH];
    for(int i = 0; i < GRATINGH; ++i)
    {
        int mid = GRATINGW / GRATRATE;
        int j = 0;
        for(; j < mid; ++j) { data[i * GRATINGW + j] = CLR_NONE; }
        for(; j < GRATINGW; ++j) { data[i * GRATINGW + j] = CLR_MASK; }
    }
    glGenTextures(1, &mData.texGrating);
    glBindTexture(GL_TEXTURE_2D, mData.texGrating);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GRATINGW, GRATINGH, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
}

#define CLR_BKGND    0.0f, 0.0f, 1.0f, 1.0f
int MultiTextureContext::run()
{
    glClearColor(CLR_BKGND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawAnimation();

    return 0;
}

void MultiTextureContext::drawAnimation()
{
    // Update mGratingTexCoords first
    static clock_t beginTime = clock();
    clock_t elapseTime = clock() - beginTime;
#if 0
    float f = ((float)((elapseTime % CLOCKS_PER_SEC) / (CLOCKS_PER_SEC / GRATRATE))) / GRATRATE;
#else
    float f = (float)(elapseTime % CLOCKS_PER_SEC) / CLOCKS_PER_SEC;
#endif
    f *= GRAT_GPS;

    // screenWidth = 480; texGratingWidth = 64; texGrantingWidthValue[0, 7.5)
    // screenHeight= 800; texGratingHeight = 1; texGrantingHeightValue[0, 800);
    float scrW = (float)getSurface()->getWidth();
    float scrH = (float)getSurface()->getHeight();
    static Coord kInitCoords[] =
    {
        {0, 0},             {scrW/GRATINGW, 0},
        {0, scrH/GRATINGH}, {scrW/GRATINGW, scrH/GRATINGH}
    };
    mGratingTexCoords[0].x = kInitCoords[0].x - f;
    mGratingTexCoords[0].y = kInitCoords[0].y;
    mGratingTexCoords[1].x = kInitCoords[1].x - f;
    mGratingTexCoords[1].y = kInitCoords[1].y;
    mGratingTexCoords[2].x = kInitCoords[2].x - f;
    mGratingTexCoords[2].y = kInitCoords[2].y;
    mGratingTexCoords[3].x = kInitCoords[3].x - f;
    mGratingTexCoords[3].y = kInitCoords[3].y;

    // begin draw
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
            mGratingTexCoords.data());

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(mProgram.samGrating, 0);
    glBindTexture(GL_TEXTURE_2D, mData.texGrating);

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(mProgram.samImage, 1);
    glBindTexture(GL_TEXTURE_2D, mData.texImage);

    static const unsigned char indices[] = { 0, 1, 2, 3};
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(mProgram.attLocations.attCoordTexGrating);
    glDisableVertexAttribArray(mProgram.attLocations.attCoordTexImage);
    glDisableVertexAttribArray(mProgram.attLocations.attCoordPos);
}
