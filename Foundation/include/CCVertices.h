#ifndef __FOUNDATION_CC__TO_STRINGVERTICES_H__
#define __FOUNDATION_CC__TO_STRINGVERTICES_H__

#include <array>
#include <vector>

class CCVertices
{
public:
    typedef CCVertices ThisType;

    enum class DataFlag : unsigned char
    {
        POSITION = 0,
        NORMAL,
        TEXTURE,
        TOTAL,
    };
    static const unsigned NO_USE = -1;

    CCVertices();

    void init(const float *vertices, unsigned len, unsigned count);

    ThisType *resetOffset(DataFlag flag = DataFlag::TOTAL, unsigned offset = NO_USE)
    {
        if (flag >= DataFlag::TOTAL)
        {
            _offsets.fill((unsigned char)offset);
        }
        else
        {
            _offsets[(unsigned)flag] = (unsigned char)offset;
        }
        return this;
    }
    int getOffset(DataFlag flag)
    {
        return (int)_offsets[(unsigned)flag];
    }
    bool hasData(DataFlag flag)
    {
        return _offsets[(unsigned)flag] != (unsigned char)NO_USE;
    }

    ::std::vector<float> &getData()
    {
        return _vertices;
    }

    unsigned getCount()
    {
        return _count;
    }
    void append(const ThisType &other)
    {
        if (true)//::std::equal(_offsets.begin(), _offsets.end(), other._offsets.begin()))
        {
            ::std::copy(other._vertices.begin(), other._vertices.end(), back_inserter(_vertices));
            _count += other._count;
        }
    }
private:
    ::std::array<unsigned char, (size_t)DataFlag::TOTAL> _offsets;
    ::std::vector<float> _vertices;
    unsigned _count;
};

#endif  // __FOUNDATION_CC__TO_STRINGVERTICES_H__
