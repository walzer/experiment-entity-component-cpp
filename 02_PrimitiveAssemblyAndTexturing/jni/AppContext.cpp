#include "AppContext.h"

#include "Debug.h"
#include "Runtime.h"
#include "OGLES2Tools.h"

AppContext::AppContext()
{
}

AppContext::~AppContext()
{
}

bool AppContext::init()
{
    // Prepare shader program and the input data.
    EPVRTError ret = PVR_SUCCESS;
    CPVRTString msg;

    do
    {
////////////////////////////////////////////////////////////////////////////////
        // 1. Create shader program.
        // 1.1 Compile vertex shader.
        const char* pszVertShader =
            "attribute highp vec4    coord;"
            "attribute highp vec4    color;"
            "uniform mediump mat4    myPMVMatrix;"
            "varying mediump vec4    v_fragmentColor;"
            "void main(void)"
            "{"
            "    gl_Position = myPMVMatrix * coord;"
            "    v_fragmentColor = color;"
            "}";
        ret = PVRTShaderLoadSourceFromMemory(
                pszVertShader,
                GL_VERTEX_SHADER,
                &mProgram.vs,
                &msg, 0, 0);
        if (ret) break;
        // 1.2 Compile fragment shader.
        const char* pszFragShader =
            "varying vec4 v_fragmentColor;"
            "void main (void)"
            "{"
            "    gl_FragColor = v_fragmentColor;"
            "}";
        ret = PVRTShaderLoadSourceFromMemory(
                pszFragShader,
                GL_FRAGMENT_SHADER,
                &mProgram.fs,
                &msg, 0, 0);
        if (ret) break;
        // 1.3 Link two shaders to program, and bind attribs to specific locations.
        const char * attribs[] =
        {
                "coord",    // index = 0
                "color"     // index = 1
        };
        mProgram.Locations.attributes.coord = 0;
        mProgram.Locations.attributes.color = 1;
        ret = PVRTCreateProgram(
                &mProgram.id,
                mProgram.vs,
                mProgram.fs,
                attribs,
                sizeof(attribs) / sizeof(attribs[0]),
                &msg);
        if (ret) break;
        // 1.4 Get uniform locations.
        mProgram.Locations.uniforms.myPMVMatrix = glGetUniformLocation(
                mProgram.id,
                "myPMVMatrix");
        if (mProgram.Locations.uniforms.myPMVMatrix < 0) break;
    } while (false);
////////////////////////////////////////////////////////////////////////////////
    // Check errors.
    if (ret)
    {
        Debug::logd("%s", msg.c_str());
    }
////////////////////////////////////////////////////////////////////////////////
    // 2. Prepare input data.
    // 2.1 Prepare uniform values.
    static float kIdentity[] =
    {
            1.0f,0.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,1.0f,0.0f,
            0.0f,0.0f,0.0f,1.0f
    };
    mData.uniforms.myPMVMatrix = kIdentity;
    // 2.2 Prepare attribute input.
    // define triangle points attributes, position(x,y,z) and color(r,g,b,a)
    static float kVertices[] =
    {
        -0.4f,-0.4f, 0.0f,      1.0f, 0.0f, 0.0f, 1.0f,
         0.4f,-0.4f, 0.0f,      0.0f, 1.0f, 0.0f, 1.0f,
         0.0f, 0.4f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f,
    };
    mData.attributes.myVertex = kVertices;
////////////////////////////////////////////////////////////////////////////////
    auto delegate = std::bind(
                &AppContext::onTouchEvent,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            );
    Runtime::getSurface(0)->pointerEvent.add(delegate);
    return true;
}

int AppContext::run()
{
////////////////////////////////////////////////////////////////////////////////
    // 1. clear background
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////////////////////////////////////////////////////////////////////////////////
    // 2. Passes the matrix to that uniform variable.
    glUniformMatrix4fv(
            mProgram.Locations.uniforms.myPMVMatrix,
            1,
            GL_FALSE,
            mData.uniforms.myPMVMatrix);
////////////////////////////////////////////////////////////////////////////////
    // 3. Passes the vertices to attribute variable.
   /* 3.1
        Enable the custom vertex attribute at index 0.
        We previously binded that index to the variable in our shader "vec4 MyVertex;"
    */
    glEnableVertexAttribArray(mProgram.Locations.attributes.coord);
    glEnableVertexAttribArray(mProgram.Locations.attributes.color);
    // 3.2 Sets the vertex data to this attribute index
    glVertexAttribPointer(
            mProgram.Locations.attributes.coord,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 7,
            mData.attributes.myVertex);
    glVertexAttribPointer(
            mProgram.Locations.attributes.color,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 7,
            mData.attributes.myVertex + 3);
////////////////////////////////////////////////////////////////////////////////
    /* 4.
        Draws a non-indexed triangle array from the pointers previously given.
        This function allows the use of other primitive types : triangle strips, lines, ...
        For indexed geometry, use the function glDrawElements() with an index list.
    */
    glDrawArrays(GL_TRIANGLES, 0, 3);
////////////////////////////////////////////////////////////////////////////////
    // 5. Cleanup context.
    glDisableVertexAttribArray(mProgram.Locations.attributes.coord);
    glDisableVertexAttribArray(mProgram.Locations.attributes.color);
    return 0;
}

void AppContext::onTouchEvent(Surface* sender, PointerData& args)
{
    Debug::logd("%s(%d)", __PRETTY_FUNCTION__, args.action);
}
