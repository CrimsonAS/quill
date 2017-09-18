#include "quill.h"

using namespace Quill;
using namespace std;

#include "dumpspans.h"


int main(int argc, char **argv)
{
    MonoRasterizer<DumpSpans, Triangle<Vertex2D>> rasterizer;

    Triangle<Vertex2D> t = Triangle<Vertex2D>::create(10, 10, 
                                                      20, 11,
                                                      15, 20);

    cout << "before sort: " << t << endl;
    cout << "after sort : " << t << endl;

    rasterizer(t);

    return 0;
}

