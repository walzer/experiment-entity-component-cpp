#include "pch.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "CCRenderManager.h"

#include "CCVertices.h"

#include "GameComponent/CCRenderComponent.h"

using namespace std;

CCRenderManager::~CCRenderManager()
{
}

// Matrix used for projection model view (PMVMatrix)
float pfIdentity[] =
{
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f
};
GLuint shaderProgram;

bool CCRenderManager::init(CCContext * context)
{
    beginDelegate = context->preDrawEvent.add(bind(&CCRenderManager::begin, this), AT_FRONT);
    drawDelegate = context->drawEvent.add(bind(&CCRenderManager::draw, this));
    postDrawDelegate = context->postDrawEvent.add(bind(&CCRenderManager::batchDraw, this));
    endDelegate = context->postDrawEvent.add(bind(&CCRenderManager::end, this));

    // context->get("CCResourceManager")->load<CCShaderProgram>("file_name");
    char* pszFragShader = "\
        varying mediump vec4 v_fragmentColor;\
        void main (void)\
        {\
            gl_FragColor = v_fragmentColor;\
        }";
    char* pszVertShader = "\
        attribute highp vec4	myVertex;\
        attribute highp vec4	myColor;\
        uniform mediump mat4	myPMVMatrix;\
        varying mediump vec4    v_fragmentColor;\
        void main(void)\
        {\
            gl_Position = myPMVMatrix * myVertex;\
            v_fragmentColor = myColor;\
        }";
    GLuint uiFragShader, uiVertShader;		/* Used to hold the fragment and vertex shader handles */
    GLuint uiProgramObject;					/* Used to hold the program handle (made out of the two previous shaders */

    // Create the fragment shader object
    uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the source code into it
    glShaderSource(uiFragShader, 1, (const char**)&pszFragShader, NULL);

    // Compile the source code
    glCompileShader(uiFragShader);

    // Check if compilation succeeded
    GLint bShaderCompiled;
    glGetShaderiv(uiFragShader, GL_COMPILE_STATUS, &bShaderCompiled);

    if (!bShaderCompiled)
    {

        // An error happened, first retrieve the length of the log message
        int i32InfoLogLength, i32CharsWritten;
        glGetShaderiv(uiFragShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

        // Allocate enough space for the message and retrieve it
        char* pszInfoLog = new char[i32InfoLogLength];
        glGetShaderInfoLog(uiFragShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

        // Displays the error in a dialog box
        CC_LOGW("FragmentShader error: %s\n", pszInfoLog);
        delete[] pszInfoLog;
    }

    // Loads the vertex shader in the same way
    uiVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(uiVertShader, 1, (const char**)&pszVertShader, NULL);
    glCompileShader(uiVertShader);
    glGetShaderiv(uiVertShader, GL_COMPILE_STATUS, &bShaderCompiled);
    if (!bShaderCompiled)
    {

        int i32InfoLogLength, i32CharsWritten;
        glGetShaderiv(uiVertShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
        char* pszInfoLog = new char[i32InfoLogLength];
        glGetShaderInfoLog(uiVertShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

        CC_LOGW("VertexShader error: %s\n", pszInfoLog);

        delete[] pszInfoLog;
    }

    // Create the shader program
    uiProgramObject = glCreateProgram();

    // Attach the fragment and vertex shaders to it
    glAttachShader(uiProgramObject, uiFragShader);
    glAttachShader(uiProgramObject, uiVertShader);

    // Bind the custom vertex attribute "myVertex" to location VERTEX_ARRAY
    glBindAttribLocation(uiProgramObject, 0, "myVertex");
    glBindAttribLocation(uiProgramObject, 1, "myColor");

    // Link the program
    glLinkProgram(uiProgramObject);

    // Check if linking succeeded in the same way we checked for compilation success
    GLint bLinked;
    glGetProgramiv(uiProgramObject, GL_LINK_STATUS, &bLinked);
    if (!bLinked)
    {
        int i32InfoLogLength, i32CharsWritten;
        glGetProgramiv(uiProgramObject, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
        char* pszInfoLog = new char[i32InfoLogLength];
        glGetProgramInfoLog(uiProgramObject, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

        CC_LOGW("LinkProgram error: %s\n", pszInfoLog);

        delete[] pszInfoLog;
    }

    shaderProgram = uiProgramObject;
    return true;
}

void CCRenderManager::done(CCContext * context)
{
    context->preDrawEvent.remove(beginDelegate);
    context->drawEvent.remove(drawDelegate);
    context->postDrawEvent.remove(endDelegate);
}

void CCRenderManager::addBatch(CCRenderComponent * com, const CCVec3 &pos)
{
    _batchVertices.emplace_back(com, pos);
}

void CCRenderManager::begin()
{
    //glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CCRenderManager::draw()
{
}

void CCRenderManager::end()
{

}

void CCRenderManager::batchDraw()
{
    if (!_batchVertices.size()) return;

    // Create vertices list.
    CCVertices vertices;
    auto &data = vertices.getData();
    CCRenderComponent *renderCom;
    for (auto com : _batchVertices)
    {
        size_t i = data.size();
        renderCom = get<0>(com);
        vertices.append(*(renderCom->getVertices()));
        const CCVec3 &position = get<1>(com);
        if (position.x == 0 && position.y == 0 && position.z == 0)
        {
            continue;
        }
        for (auto end = data.size(); i < end; i += 6)
        {
            data[i] += position.x;
            data[i + 1] += position.y;
        }
    }

    // Begin draw.

    // Use program and set gl status.
    // shaderProgram.bind();
    glUseProgram(shaderProgram);

    // set MVP transform matrix.
    int i32Location = glGetUniformLocation(shaderProgram, "myPMVMatrix");
    glUniformMatrix4fv(i32Location, 1, GL_FALSE, pfIdentity);

    // texture.bind();

    // Apply blindfunc if needed.

    // Bind mesh data.
    // vertices.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, vertices.getData().data());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, vertices.getData().data() + 3);

    // Draw.
    // mesh.draw();
    glDrawArrays(GL_TRIANGLES, 0, vertices.getCount());

    // Clear status.
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    _batchVertices.clear();
}
