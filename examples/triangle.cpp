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

    void operator()(Vertex pos, int length, VaryingUV v, VaryingUV dx);
};


void Consumer::operator()(Vertex pos, int length, VaryingUV v, VaryingUV dx)
{
    unsigned int *dst = buffer.scanline((int) pos.y) + (int) pos.x;

    for (int i=0; i<length; ++i) {
        int r = int(255 * v.u);
        int b = int(255 * v.v);
        dst[i] += 0xff000000 | (r) | (b << 16);
        dst[i] += 0x00003f00;
        v = v + dx;
    }
}


int main(int argc, char **argv)
{
    LerpRaster<Consumer> raster;

    RasterBuffer *buffer = &raster.fill.buffer;
    buffer->allocate(100, 100);
    buffer->fill(0xff000000);

    raster(Triangle(Vertex(10, 10), Vertex(90, 50), Vertex(40, 90)), VaryingUV(1, 1), VaryingUV(1, 0), VaryingUV(0, 1));
    raster(Triangle(Vertex(10, 10), Vertex(90, 50), Vertex(95,  5)), VaryingUV(1, 1), VaryingUV(1, 0), VaryingUV(0, 0));
    raster(Triangle(Vertex(10, 10), Vertex( 8, 92), Vertex(40, 90)), VaryingUV(1, 1), VaryingUV(0, 0), VaryingUV(0, 1));
    raster(Triangle(Vertex(93, 93), Vertex(90, 50), Vertex(40, 90)), VaryingUV(0, 0), VaryingUV(1, 0), VaryingUV(0, 1));

    stbi_write_png("triangle.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
}
