#ifndef RUNTIME_H_
#define RUNTIME_H_

class Surface;

struct RuntimeListener
{
    RuntimeListener();
    virtual ~RuntimeListener();
    virtual void onSurfaceCreated(Surface * surface) = 0;
};

class Runtime
{
public:
    static Surface * getSurface(int id);

protected:
    static int addSurface(Surface* surface);

private:
    Runtime();
};

#endif /* RUNTIME_H_ */
