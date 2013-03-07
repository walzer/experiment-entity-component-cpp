#ifndef __RENDER_H__
#define __RENDER_H__

class Context;

class Surface
{
public:
    virtual ~Surface();

    Context * getContext();
    void setContext(Context * context);

    int getWidth();
    int getHeight();
protected:
    Surface();

    Context * mContext;
    int mWidth;
    int mHeight;
};

#endif /* __RENDER_H__ */
