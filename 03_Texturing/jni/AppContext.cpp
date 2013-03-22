#include "AppContext.h"

#include "Debug.h"
#include "Runtime.h"
#include "SampleTools.h"

#include "OGLES2Tools.h"

using namespace std;

AppContext::AppContext()
{
}

AppContext::~AppContext()
{
}

bool AppContext::init()
{
    // Initialize touch event delegate.
    auto delegate = std::bind(&AppContext::onTouchEvent, this, placeholders::_1, placeholders::_2);
    Runtime::getSurface(0)->pointerEvent.add(delegate);

    auto onClick = [this](Surface * sender, PointerArgs& args)
        {
            Debug::logd("click at: %f, %f", args.x, args.y);
            if (this->mPositionArray.size() >= MAX_SIZE)
            {
                this->mPositionArray.clear();
            }
            else
            {
                this->mPositionArray.push_back(args.x);
                this->mPositionArray.push_back(args.y);
            }
        };
    clickEvent.clickEvent.add(onClick);

    // Initialize texturing program.
    const char* vss =
        "attribute  highp   vec2    attPosCoord;"
//        "attribute  highp   vec2    attTexCoord;"
        "uniform    mediump float   uniPointSize;"
//        "varying    mediump vec2    varTexCoord;"
        "void main(void)"
        "{"
        "    gl_Position = vec4(attPosCoord.x, attPosCoord.y, 0.0f, 1.0f);"
        "    gl_PointSize = uniPointSize;"
//        "    varTexCoord = attTexCoord;"
        "}";
    const char* fss =
        "uniform    sampler2D       samTexture2d;"
//        "varying    mediump vec2    varTexCoord;"
        "void main (void)"
        "{"
        "    gl_FragColor = texture2D(samTexture2d, gl_PointCoord);"
//        "    gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
        "}";
    const char * attribs[] =
    {
            "attPosCoord",      // index = 0
            "attTexCoord",      // index = 1
    };
    mProgram.id = createProgramFromSource(
            vss,
            fss,
            attribs,
            (unsigned int *)&mProgram.attLocations,
            1);
    mProgram.uniPointSize = glGetUniformLocation(mProgram.id, "uniPointSize");
    mProgram.samTexture2d = glGetUniformLocation(mProgram.id, "samTexture2d");

    // Initialize data.
    mData.pointSize = 128;
    if(PVRTTextureLoadFromPVR("texPoint.pvr", &mData.texPoint) != PVR_SUCCESS)
    {
        Debug::logd("PVRTTextureLoadFromPVR failed!!");
    }
    return true;
}

int AppContext::run()
{
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgram.id);

    glEnableVertexAttribArray(mProgram.attLocations.attPosCoord);
    //glEnableVertexAttribArray(mProgram.attLocations.attTexCoord);
    if (mPositionArray.size())
        Debug::logd("Draw at point(%f, %f)",mPositionArray[0],mPositionArray[1]);
    glVertexAttribPointer(
            mProgram.attLocations.attPosCoord,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mPositionArray.data());

    static void (AppContext::*drawTextures[])() =
    {
        &AppContext::drawPoints,        // 0
    };
    // Select a draw function.
    (this->*drawTextures[0])();

    glDisableVertexAttribArray(mProgram.attLocations.attPosCoord);
    return 0;
}

void AppContext::drawPoints()
{
    glUniform1f(mProgram.uniPointSize, mData.pointSize);
    glBindTexture(GL_TEXTURE_2D, mData.texPoint);
    glDrawArrays(GL_POINTS, 0, mPositionArray.size() / 2);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void AppContext::onTouchEvent(Surface* sender, PointerData& args)
{
    Debug::logd("%s(%d)(%f, %f)", __PRETTY_FUNCTION__,
            args.action, args.args[0].x, args.args[0].y);
    clickEvent(sender, args);
}
