#ifndef __APP_CONTEXT_H__
#define __APP_CONTEXT_H__

#include "Context.h"
#include "Surface.h"

#include "GLES2/gl2.h"

#include <vector>

class AppContext;

class ClickRecognizer
{
public:
    EventHandler<Surface, PointerArgs> clickEvent;
    void operator () (Surface* sender, PointerData& args)
    {
        switch (args.action)
        {
        case ACTION_DOWN:
            mCapture = true;
            mArgs = args.args[0];
            break;

        case ACTION_UP:
            if (mCapture == true &&
                mArgs.id == args.args[0].id &&
                mArgs.x == args.args[0].x &&
                mArgs.y == args.args[0].y)
            {
                // Convert windows coordinate to gl coordinate
                mArgs.x = 2 * (mArgs.x / sender->getWidth()) - 1.0f;
                mArgs.y = 2 * ((sender->getHeight() - mArgs.y) / sender->getHeight()) - 1.0f;
                clickEvent.raise(sender, mArgs);
            }
            break;

        default:
            break;
        }
    }
private:
    bool  mCapture;
    PointerArgs mArgs;
};
class AppContext : public Context
{
public:
    AppContext();
    virtual ~AppContext();

    virtual bool init();
    virtual int run();

    ClickRecognizer clickEvent;
    void onTouchEvent(Surface* sender, PointerData& args);

protected:
    void drawPoints();
    void drawLines();
    void drawLineLoop();
    void drawLineStrip();
    void drawTrangles();
    void drawTrangleStrip();
    void drawTrangleFan();

    // Store the touch positions(x,y), at most 6.
    static const unsigned MAX_SIZE = 6 * 2;
    std::vector<float> mPositionArray;

    // Program identifier for primitive assembly.
    struct
    {
        GLuint id;
        GLuint attrCoord;
        GLuint unifColor;
    } mProgPrimitive;
};

#endif /* __APP_CONTEXT_H__ */
