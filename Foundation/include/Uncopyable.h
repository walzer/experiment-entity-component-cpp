#ifndef __UNCOPYABLE_H__
#define __UNCOPYABLE_H__

namespace cc {;

class Uncopyable
{
    Uncopyable(const Uncopyable &);
    Uncopyable &operator = (const Uncopyable &);
public:
    Uncopyable() {}
};

}   // namespace cc

#endif  // __UNCOPYABLE_H__
