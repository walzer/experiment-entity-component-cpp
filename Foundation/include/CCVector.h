#ifndef __FOUNDATION__CCVECTOR_H__
#define __FOUNDATION__CCVECTOR_H__

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

#endif  // __FOUNDATION__CCVECTOR_H__
