#include "pch.h"

#include "CCVertices.h"

using namespace std;

CCVertices::CCVertices()
    : _count()
{
}

void CCVertices::init(const float *vertices, unsigned len, unsigned count)
{
    _vertices.assign(vertices, vertices + len);
    _count = count;
}
