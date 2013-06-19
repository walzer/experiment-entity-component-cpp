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

class Unnewable {
protected:
    void *operator new (size_t) { return nullptr; }
    void operator delete (void *) {}
private:
    void *operator new[] (size_t);
    void operator delete[] (void *);
};

}   // namespace cc

#endif  // __UNCOPYABLE_H__
