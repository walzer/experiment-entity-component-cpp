#include "HelloContext.h"

#include "Debug.h"

#include "OGLES2Tools.h"

HelloContext::HelloContext()
{
}

HelloContext::~HelloContext()
{
}

bool HelloContext::init()
{
    EPVRTError ret = PVR_SUCCESS;
    CPVRTString msg;

    do
    {
        const char* pszVertShader = "\
            attribute highp vec4    myVertex;\
            uniform mediump mat4    myPMVMatrix;\
            void main(void)\
            {\
                gl_Position = myPMVMatrix * myVertex;\
            }";
        ret = PVRTShaderLoadSourceFromMemory(
                pszVertShader,
                GL_VERTEX_SHADER,
                &mProgram.vs,
                &msg, 0, 0);
        if (ret) break;
////////////////////////////////////////////////////////////////////////////////
        const char* pszFragShader = "\
            void main (void)\
            {\
                gl_FragColor = vec4(1.0, 1.0, 0.66 ,1.0);\
            }";
        ret = PVRTShaderLoadSourceFromMemory(
                pszFragShader,
                GL_FRAGMENT_SHADER,
                &mProgram.fs,
                &msg, 0, 0);
        if (ret) break;
        ////////////////////////////////////////////////////////////////////////////////
        const char * attribs[] =
        {
                "myVertex"  // index = 0
        };
        mProgram.Locations.attributes.myVertex = 0;
        ret = PVRTCreateProgram(
                &mProgram.id,
                mProgram.vs,
                mProgram.fs,
                attribs,
                sizeof(attribs) / sizeof(attribs[0]),
                &msg);
        if (ret) break;
        ////////////////////////////////////////////////////////////////////////////////
        mProgram.Locations.uniforms.myPMVMatrix = glGetUniformLocation(
                mProgram.id,
                "myPMVMatrix");
        if (mProgram.Locations.uniforms.myPMVMatrix < 0) break;
        ////////////////////////////////////////////////////////////////////////////////
        static float kIdentity[] =
        {
                1.0f,0.0f,0.0f,0.0f,
                0.0f,1.0f,0.0f,0.0f,
                0.0f,0.0f,1.0f,0.0f,
                0.0f,0.0f,0.0f,1.0f
        };
        mData.uniforms.myPMVMatrix = kIdentity;
        // define triangle points positions(x,y,z)
        static float kVertices[] =
        {
            -0.4f,-0.4f,0.0f,
            0.4f ,-0.4f,0.0f,
            0.0f ,0.4f ,0.0f
        };
        mData.attributes.myVertex = kVertices;
    } while (false);

    if (ret)
    {
        Debug::logd("%s", msg.c_str());
    }

    return true;
}

int HelloContext::run()
{
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Passes the matrix to that variable.
    glUniformMatrix4fv(
            mProgram.Locations.uniforms.myPMVMatrix,
            1,
            GL_FALSE,
            mData.uniforms.myPMVMatrix);
    /*
        Enable the custom vertex attribute at index VERTEX_ARRAY.
        We previously binded that index to the variable in our shader "vec4 MyVertex;"
    */
    glEnableVertexAttribArray(mProgram.Locations.attributes.myVertex);

    // Sets the vertex data to this attribute index
    glVertexAttribPointer(
            mProgram.Locations.attributes.myVertex,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            mData.attributes.myVertex);

    /*
        Draws a non-indexed triangle array from the pointers previously given.
        This function allows the use of other primitive types : triangle strips, lines, ...
        For indexed geometry, use the function glDrawElements() with an index list.
    */
    glDrawArrays(GL_TRIANGLES, 0, 3);
    return 0;
}
