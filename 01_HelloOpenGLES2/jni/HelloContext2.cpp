#include "HelloContext2.h"

bool HelloContext2::init()
{
    HelloContext::init();

    static float kCoords[] =
    {
        -0.4f,-0.4f, 0.0f,
         0.4f,-0.4f, 0.0f,
         0.0f, 0.4f, 0.0f,
    };
    mCoordsArray = kCoords;
    static float kColors[] =
    {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };
    mColorsArray = kColors;
    return true;
}

int HelloContext2::run()
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
            sizeof(float) * 3,
            mCoordsArray);
    glVertexAttribPointer(
            mProgram.Locations.attributes.color,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 4,
            mColorsArray);
////////////////////////////////////////////////////////////////////////////////
    /* 4.
        Draws a non-indexed triangle array from the pointers previously given.
        This function allows the use of other primitive types : triangle strips, lines, ...
        For indexed geometry, use the function glDrawElements() with an index list.
    */
    static unsigned char indices[] = { 0, 1, 2};
    glDrawElements(
            GL_TRIANGLES,
            sizeof(indices)/sizeof(indices[0]),
            GL_UNSIGNED_BYTE,
            indices);
////////////////////////////////////////////////////////////////////////////////
    // 5. Cleanup context.
    glDisableVertexAttribArray(mProgram.Locations.attributes.coord);
    glDisableVertexAttribArray(mProgram.Locations.attributes.color);
    return 0;
}
