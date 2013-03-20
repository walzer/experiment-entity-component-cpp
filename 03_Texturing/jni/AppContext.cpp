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
    mProgPrimitive.id = createProgramFromSource(
            primitiveVss,
            primitiveFss,
            primitiveAttribs,
            &mProgPrimitive.attrCoord,
            1);
    mProgPrimitive.unifColor = glGetUniformLocation(mProgPrimitive.id, "color");
    mProgPrimitive.unifPSize = glGetUniformLocation(mProgPrimitive.id, "pointSize");

    return true;
}

int AppContext::run()
{
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgPrimitive.id);

    glEnableVertexAttribArray(mProgPrimitive.attrCoord);
    glVertexAttribPointer(
            mProgPrimitive.attrCoord,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            mPositionArray.data());

    static void (AppContext::*drawPrimitive[])() =
    {
        &AppContext::drawPoints,        // 0
    };
    // Select a draw function.
    (this->*drawPrimitive[0])();

    glDisableVertexAttribArray(mProgPrimitive.attrCoord);
    return 0;
}

void AppContext::drawPoints()
{
    glUniform1f(mProgPrimitive.unifPSize, 7.0f);
    glUniform4f(mProgPrimitive.unifColor, 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, mPositionArray.size() / 2);
}

void AppContext::onTouchEvent(Surface* sender, PointerData& args)
{
    Debug::logd("%s(%d)(%f, %f)", __PRETTY_FUNCTION__,
            args.action, args.args[0].x, args.args[0].y);
    clickEvent(sender, args);
}
