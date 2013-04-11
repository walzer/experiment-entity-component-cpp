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
        "attribute  highp   vec2    attCoordPosNext;"
        "attribute  highp   vec2    attCoordTex;"
        "uniform    highp   float   uniStep;"
        "varying    mediump vec2    varCoordTex;"
        "void main(void)"
        "{"
        "    vec2 pos = attCoordPos * (1 - uniStep) + attCoordPosNext * uniStep;"
        "    gl_Position = vec4(pos, 0, 1);"
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
            "attCoordPosNext",      // index = 1
            "attCoordTex",          // index = 2
    };
    mProgram.id = createProgramFromSource(
            vss,
            fss,
            attribs,
            (unsigned int *)&mProgram.attLocations.attCoordPos,
            3);

    Debug::logd("Program(%d)(%d, %d)",
            mProgram.id,
            mProgram.attLocations.attCoordPos,
            mProgram.attLocations.attCoordTex);
    mProgram.samShark = glGetUniformLocation(mProgram.id, "samShark");
    mProgram.uniStep = glGetUniformLocation(mProgram.id, "uniStep");
}

static float s_texW = 384;
static float s_texH = 144;
void AppContext::loadIndicesAttribs()
{
    static float points[] =
    {
            1, 94,      48, 144,    72, 96,     82, 144,    110, 96,
            152, 140,   152, 96,    188, 142,   212, 106,   382, 84,
            382, 60,    212, 40,    192, 2,     154, 50,    152, 2,
            110, 46,    66, 48,     64, 2,      30, 2,      1, 58,
    };
#define pointsCount sizeof(points) / sizeof(points[0])
    static unsigned char indices[] =
    {
            0, 1, 2,    1, 2, 3,    4, 5, 6,    5, 6, 7,
            0, 2, 19,   2, 16, 19,  2, 4, 16,   4, 15, 16,  4, 6, 15,
            6, 13, 15,  6, 8, 13,   8, 11, 13,  8, 9, 11,   9, 10, 11,
            12, 13, 14, 13, 14, 15, 16, 17, 18, 16, 18, 19,
    };
    mData.indices = indices;
    mData.indicesCount = sizeof(indices) / sizeof(indices[0]);
    // Calculate texture coords in image.
    static float coordTexs[pointsCount];
    for (int i = 0; i < pointsCount; i += 2)
    {
        coordTexs[i] = points[i] / s_texW;
        coordTexs[i + 1] = (s_texH - points[i + 1]) / s_texH;
        Debug::logd("coordTexs[%d](%f, %f)", i, coordTexs[i], coordTexs[i+1]);
    }
    mData.coordTexs = coordTexs;
    // Calculate position coords for frames.
    float fishW = s_texW * 2 / 480;
    float fishH = s_texH * 2 / 800;
    auto convertPt2Coord = [fishW, fishH](float w, float h, float& cw, float& ch)
        {
            cw = fishW * (w / s_texW - 0.5f);
            ch = fishH * ((s_texH - h) / s_texH - 0.5f);
        };
    static float coordPossFrame0[pointsCount];
    for (int i = 0; i < pointsCount; i += 2)
    {
        convertPt2Coord(
                points[i],
                points[i + 1],
                coordPossFrame0[i],
                coordPossFrame0[i + 1]);
    }
    mData.coordPossFrames[0] = coordPossFrame0;

    static float pointsOffset1[] =
    {
            2, 8,   4, 4,   2, 4,   4, 2,   1, 1,
            4, -3,  0, 1,   3, -4,  -3, 5,  -4, 35,
            0, 26,  2, 6,   0, -2,  -1, -2, -3, -2,
            -1, 1,  -2, 3,  -4, 4,  -4, 6,  -1, 8,
    };
    static float coordPossFrame1[pointsCount];
    for (int i = 0; i < pointsCount; i += 2)
    {
        convertPt2Coord(
                points[i] + pointsOffset1[i],
                points[i + 1] + pointsOffset1[i + 1],
                coordPossFrame1[i],
                coordPossFrame1[i + 1]);
    }
    mData.coordPossFrames[1] = coordPossFrame1;

    static float pointsOffset2[] =
    {
            3, 12,  6, 5,   3, 6,   6, 2.5, 1.5, 1.5,
            5, -4,  0, 1.5, 4, -5,  -6, 10,  -16, 64,
            -5, 65, 10, 16,  -4, -4,  -3, -3, -5, -2,
            -2, 2,  -3, 5,  -7, 7,  -8, 9,  -2, 12,
    };
    static float coordPossFrame2[pointsCount];
    for (int i = 0; i < pointsCount; i += 2)
    {
        convertPt2Coord(
                points[i] + pointsOffset2[i],
                points[i + 1] + pointsOffset2[i + 1],
                coordPossFrame2[i],
                coordPossFrame2[i + 1]);
    }
    mData.coordPossFrames[2] = coordPossFrame2;
    mData.coordPossFrames[3] = coordPossFrame1;
    mData.coordPossFrames[4] = coordPossFrame0;

    static float coordPossFrame5[pointsCount];
    for (int i = 0; i < pointsCount; i += 2)
    {
        convertPt2Coord(
                points[i] - pointsOffset1[i],
                points[i + 1] - pointsOffset1[i + 1],
                coordPossFrame5[i],
                coordPossFrame5[i + 1]);
    }
    mData.coordPossFrames[5] = coordPossFrame5;

    static float coordPossFrame6[pointsCount];
    for (int i = 0; i < pointsCount; i += 2)
    {
        convertPt2Coord(
                points[i] - pointsOffset2[i],
                points[i + 1] - pointsOffset2[i + 1],
                coordPossFrame6[i],
                coordPossFrame6[i + 1]);
    }
    mData.coordPossFrames[6] = coordPossFrame6;
    mData.coordPossFrames[7] = coordPossFrame5;
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
    // Update mipmap scale factor
    static clock_t beginTime = clock();
    clock_t elapseTime = clock() - beginTime;
    float t = (float)(elapseTime % (CLOCKS_PER_SEC * FRAME_COUNT)) / CLOCKS_PER_SEC;
    Debug::logd("%f", t);
    int frameIdx = (int)t;
    float step = t - frameIdx;

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
            mData.coordPossFrames[frameIdx]);
    glEnableVertexAttribArray(mProgram.attLocations.attCoordPosNext);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordPosNext,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mData.coordPossFrames[(frameIdx + 1) % FRAME_COUNT]);
    // Assign mipmap texture coordinate attribute
    glEnableVertexAttribArray(mProgram.attLocations.attCoordTex);
    glVertexAttribPointer(
            mProgram.attLocations.attCoordTex,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mData.coordTexs);

    glUniform1f(mProgram.uniStep, step);

    glBindTexture(GL_TEXTURE_2D, mData.texShark);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawElements(GL_TRIANGLES, mData.indicesCount, GL_UNSIGNED_BYTE, mData.indices);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(mProgram.attLocations.attCoordTex);
    glDisableVertexAttribArray(mProgram.attLocations.attCoordPos);

}
