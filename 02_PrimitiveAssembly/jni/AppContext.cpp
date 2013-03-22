#include "AppContext.h"

#include "Debug.h"
#include "Runtime.h"
#include "SampleTools.h"

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

    // Initialize primitive assembly program.
    const char* primitiveVss =
        "attribute  highp   vec4    coord;"
        "uniform    mediump float   pointSize;"
        "void main(void)"
        "{"
        "    gl_Position = coord;"
        "    gl_PointSize = pointSize;"
        "}";
    const char* primitiveFss =
        "uniform vec4    color;"
        "void main (void)"
        "{"
        "    gl_FragColor = color;"
        "}";
    const char * primitiveAttribs[] =
    {
            "coord",    // index = 0
    };
    mProgram.id = createProgramFromSource(
            primitiveVss,
            primitiveFss,
            primitiveAttribs,
            &mProgram.attrCoord,
            1);
    mProgram.unifColor = glGetUniformLocation(mProgram.id, "color");
    mProgram.unifPSize = glGetUniformLocation(mProgram.id, "pointSize");

    mAnotherStrip =
    {
            0.25f, -0.25f,      0.75f, -0.25f,
            0.25f, -0.75f,      0.75f, -0.75f,
    };
    // Get some limits.
    float   pointSizeRange[2];
    glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, pointSizeRange);
    Debug::logd("GLES: point size range:(%f, %f)", pointSizeRange[0], pointSizeRange[1]);

    GLfloat   lineWidthRange[2];
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
    Debug::logd("GLES: line width range:(%f, %f)", lineWidthRange[0], lineWidthRange[1]);
    return true;
}

int AppContext::run()
{
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgram.id);

    glEnableVertexAttribArray(mProgram.attrCoord);
    glVertexAttribPointer(
            mProgram.attrCoord,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mPositionArray.data());

    static void (AppContext::*drawPrimitive[])() =
    {
        &AppContext::drawPoints,        // 0
        &AppContext::drawLines,         // 1
        &AppContext::drawLineLoop,      // 2
        &AppContext::drawLineStrip,     // 3
        &AppContext::drawTrangles,      // 4
        &AppContext::drawTrangleStrip,  // 5
        &AppContext::drawTrangleFan,    // 6
        &AppContext::drawConnectedStrip,    // 7
    };
    // Select a draw function.
    (this->*drawPrimitive[0])();

    glDisableVertexAttribArray(mProgram.attrCoord);
    return 0;
}

void AppContext::drawPoints()
{
    glUniform1f(mProgram.unifPSize, 7.0f);
    glUniform4f(mProgram.unifColor, 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, mPositionArray.size() / 2);
}

void AppContext::drawLines()
{
    glUniform4f(mProgram.unifColor, 0.0f, 1.0f, 0.0f, 1.0f);
    glLineWidth(3);
    glDrawArrays(GL_LINES, 0, mPositionArray.size() / 2);
}

void AppContext::drawLineLoop()
{
    glUniform4f(mProgram.unifColor, 0.0f, 1.0f, 0.0f, 1.0f);
    glLineWidth(3);
    glDrawArrays(GL_LINE_LOOP, 0, mPositionArray.size() / 2);
}

void AppContext::drawLineStrip()
{
    glUniform4f(mProgram.unifColor, 0.0f, 1.0f, 0.0f, 1.0f);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, mPositionArray.size() / 2);
}

void AppContext::drawTrangles()
{
    glUniform4f(mProgram.unifColor, 0.0f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, mPositionArray.size() / 2);
}

void AppContext::drawTrangleStrip()
{
    glUniform4f(mProgram.unifColor, 0.0f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, mPositionArray.size() / 2);
}

void AppContext::drawTrangleFan()
{
    glUniform4f(mProgram.unifColor, 0.0f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, mPositionArray.size() / 2);
}

void AppContext::drawConnectedStrip()
{
    if(mPositionArray.size() == 4 * 2)
    {
        float lastX = mPositionArray[mPositionArray.size() - 2];
        float lastY = mPositionArray[mPositionArray.size() - 1];
        mPositionArray.push_back(lastX);
        mPositionArray.push_back(lastY);
        mPositionArray.push_back(mAnotherStrip[0]);
        mPositionArray.push_back(mAnotherStrip[1]);
        mPositionArray.insert(
                mPositionArray.end(),
                mAnotherStrip.begin(),
                mAnotherStrip.end());
    }
    drawTrangleStrip();
}

void AppContext::onTouchEvent(Surface* sender, PointerData& args)
{
    Debug::logd("%s(%d)(%f, %f)", __PRETTY_FUNCTION__,
            args.action, args.args[0].x, args.args[0].y);
    clickEvent(sender, args);
}
