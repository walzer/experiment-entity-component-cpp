#ifndef HELLOCONTEXT_H_
#define HELLOCONTEXT_H_

#include "Context.h"

#include "GLES2/gl2.h"

class HelloContext : public Context
{
public:
    HelloContext();
    virtual ~HelloContext();

    virtual bool init();
    virtual int run();

private:
    struct
    {
        GLuint  id;
        GLuint  vs;
        GLuint  fs;
        struct
        {
            struct
            {
                int myPMVMatrix;
            } uniforms;
            struct
            {
                GLuint myVertex;
            } attributes;
        } Locations;

    } mProgram;

    struct
    {
        struct
        {
            const float * myPMVMatrix;
        } uniforms;
        struct
        {
            const float * myVertex;
        } attributes;
    }mData;
};

#endif /* HELLOCONTEXT_H_ */
