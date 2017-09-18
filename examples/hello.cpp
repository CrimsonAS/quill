#include "quill.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Quill;
using namespace std;

#include "rasterbuffer.h"
#include "solidcolorfiller.h"


// struct AccumulateInRaster
// {
//     RasterBuffer buffer;
//     unsigned int value;

//     void operator()(Span span);
// };


// void AccumulateInRaster::operator()(Span span)
// {
//     if (span.y < 0 || span.y >= buffer.height) {
//         return;
//     }

//     if (span.x < 0) {
//         span.length += span.x;
//         span.x = 0;
//     }

//     if (span.x + span.length >= buffer.width) {
//         span.length = buffer.width - span.x;
//     }

//     if (span.length > 0) {
//         unsigned int *sline = buffer.scanline(span.y) + span.x;
//         for (int x=0; x<span.length; ++x) {
//             sline[x] += value;
//         }
//     }
// }



int main(int argc, char **argv)
{
    Stroker<MonoRasterizer<SolidColorFiller, Triangle<Vertex2D>>> stroker;

    SolidColorFiller *fill = &stroker.rasterizer.fill;
    fill->value = 0xffe0b0a0;

    RasterBuffer *buffer = &fill->buffer;
    buffer->allocate(100, 100);
    buffer->fill(0xff000000);

    stroker.width = 16;
    stroker.moveTo(10, 10);
    stroker.lineTo(90, 10);
    stroker.width = 8;
    stroker.lineTo(90, 90);
    stroker.lineTo(10, 90);
    stroker.close();

    stbi_write_png("output.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));


    return 0;
}
