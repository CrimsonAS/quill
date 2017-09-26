#include "quill.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "rasterbuffer.h"
#include "solidcolorfill.h"

using namespace Quill;
using namespace std;

struct Consumer
{
    typedef VaryingUV Varyings;

    RasterBuffer buffer;

    void operator()(VertexUV, int length, VaryingUV dx);
};


void Consumer::operator()(VertexUV left, int length, VaryingUV dx)
{
    unsigned int *dst = buffer.scanline((int) left.y) + (int) left.x;

    Varyings vd = left.v;

    for (int i=0; i<length; ++i) {
        int r = int(255 * vd.u);
        int b = int(255 * vd.v);
        dst[i] += 0xff000000 | (r) | (b << 16);
        dst[i] += 0x00003f00;
        vd = vd + dx;
    }
}


int main(int argc, char **argv)
{
}
