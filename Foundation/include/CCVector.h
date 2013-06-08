#ifndef __FOUNDATION_CC__TO_STRINGVECTOR_H__
#define __FOUNDATION_CC__TO_STRINGVECTOR_H__

#include <array>

class CCVec3
{
public:
    union
    {
        ::std::array<float, 3> array;
        struct
        {
            float x;
            float y;
            float z;
        };
    };

    float & operator [] (size_t pos)
    {
        return array[pos];
    }
};

#endif  // __FOUNDATION_CC__TO_STRINGVECTOR_H__
