#include "quill.h"

using namespace Quill;
using namespace std;

#include "dumpspans.h"


int main(int argc, char **argv)
{
    MonoRasterizer<DumpSpans> rasterizer;

    Triangle t;
    t.x0 = 797.141662;
    t.y0 = 690.499755;
    t.x1 = 805.431763;
    t.y1 = 696.086426;
    t.x2 = 805.291076;
    t.y2 = 696.295166;

    rasterizer(t);

    return 0;
}

