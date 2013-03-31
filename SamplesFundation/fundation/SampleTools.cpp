#include "SampleTools.h"

#include "Debug.h"

#include "OGLES2Tools.h"

unsigned createProgramFromSource(
        const char *    vss,        // vertex shader source
        const char *    fss,        // fragment shader source
        const char**    attribs,    // attribute list
        unsigned *      attrLocal,  // store attribute locations
        const unsigned  num         // attribute list size
    )
{
    unsigned id = 0;
    unsigned vs = 0;
    unsigned fs = 0;
    EPVRTError ret = PVR_SUCCESS;
    CPVRTString msg;
    int step = 0;
    do
    {
        if (!vss || !fss || (num && (!attribs || !attrLocal)))
        {
            Debug::logd("There's no shader source!!");
            break;
        }
        // 1 Compile vertex shader.
        step = 1;
        if (PVR_SUCCESS != PVRTShaderLoadSourceFromMemory(
                vss, GL_VERTEX_SHADER, &vs, &msg, 0, 0))
            break;
        // 2 Compile fragment shader.
        step = 2;
        if (PVR_SUCCESS != PVRTShaderLoadSourceFromMemory(
                fss, GL_FRAGMENT_SHADER, &fs, &msg, 0, 0))
            break;
        // 3 Link two shaders to program, and bind attribs to specific locations.
        step = 3;
        if (PVR_SUCCESS != PVRTCreateProgram( &id, vs, fs, attribs, num, &msg))
            break;
        for (unsigned i = 0; i < num; i++)  *attrLocal = i;
    } while (false);
    if (! id)
    {
        Debug::logd("(step:%d)%s", step, msg.c_str());
    }
    return id;
}
