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
    LerpRaster<Consumer> raster;

    RasterBuffer *buffer = &raster.fill.buffer;
    buffer->allocate(100, 100);
    buffer->fill(0xff000000);

    raster(Triangle<VertexUV>(VertexUV(10, 10, VaryingUV(1, 1)),
                              VertexUV(90, 50, VaryingUV(1, 0)),
                              VertexUV(40, 90, VaryingUV(0, 1))));

    raster(Triangle<VertexUV>(VertexUV(10, 10, VaryingUV(1, 1)),
                              VertexUV(90, 50, VaryingUV(1, 0)),
                              VertexUV(95,  5, VaryingUV(0, 0))));

    raster(Triangle<VertexUV>(VertexUV(10, 10, VaryingUV(1, 1)),
                              VertexUV( 8, 92, VaryingUV(0, 0)),
                              VertexUV(40, 90, VaryingUV(0, 1))));

    raster(Triangle<VertexUV>(VertexUV(93, 93, VaryingUV(0, 0)),
                              VertexUV(90, 50, VaryingUV(1, 0)),
                              VertexUV(40, 90, VaryingUV(0, 1))));

    stbi_write_png("triangle.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
}
