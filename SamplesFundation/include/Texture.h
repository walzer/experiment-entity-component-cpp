#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <memory>

struct Texture
{
    typedef std::shared_ptr<Texture>    Handler;
    typedef enum
    {
        FMT_RGBA = 0,
    } Format;

    Texture()
    : mId(0), mWidth(0), mHeight(0), mAlphaEnabled(false), mPremultiplied(false)
    {

    }
    unsigned getId()        {return mId;}
    unsigned getWidth()     {return mWidth;}
    unsigned getHeight()    {return mHeight;}
    bool alphaEnabled()     {return mAlphaEnabled;}
    bool premultiplied()    {return mPremultiplied;}

private:
    unsigned mId;
    unsigned short mWidth;
    unsigned short mHeight;
    bool mAlphaEnabled;
    bool mPremultiplied;
};

#endif /* TEXTURE_H_ */
