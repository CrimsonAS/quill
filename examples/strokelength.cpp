#include "quill.h"

#include <cmath>

#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "rasterbuffer.h"
#include "solidcolorfill.h"

using namespace Quill;
using namespace std;
using namespace std::chrono;

struct FbmFill
{
    typedef VaryingUV Varyings;
    RasterBuffer buffer;
    void operator()(Vertex pos, int length, VaryingUV v, VaryingUV dx);
};



void FbmFill::operator()(Vertex pos, int length, Varyings v, VaryingUV dx)
{
    unsigned int *dst = buffer.scanline((int) pos.y) + (int) pos.x;
    for (int i=0; i<length; ++i) {
        float n = stb_perlin_fbm_noise3(v.u, v.v, 0, // x/y/z
                                        2.0,         // lacunarity
                                        0.5,         // gain
                                        6,           // octaves
                                        0, 0, 0);    // x/y/z wrap
        if (n > 0) {
            int x = n * 255;
            dst[i] = 0xff000000 | x | (x << 8) | (x << 16);
        }

        v = v + dx;
    }
}



void doFbmStroke()
{
    Stroker<LerpRaster<FbmFill>, VaryingGeneratorLengthWidth> stroker;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->allocate(800, 800);
    buffer->fill(0xff000000);

    const int COUNT = 100;

    time_point<std::chrono::system_clock> timeStart = system_clock::now();

    for (int i=0; i<COUNT; ++i) {

        float t = i / float(COUNT);

        float x = 100 + t * 600;
        float y = 600 + 100 * cos(4 * t * M_PI) - t * 400;

        stroker.width = 10 + 10 * std::abs(cos(4 * t * M_PI));

        if (i == 0) {
            stroker.moveTo(x, y);
        } else {
            stroker.lineTo(x, y);
        }
    }

    time_point<std::chrono::system_clock> timeEnd = system_clock::now();
    cout << "Fancy noise fill stroke completed in: " << duration<double>(timeEnd - timeStart).count() << " seconds" << endl
         << " - length: " << stroker.length << " pixels.." << endl
         << " - triangles: " << stroker.triangleCount << endl
         << " - segments: " << COUNT << endl;


    stbi_write_png("strokelength_fbm.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
}

struct SimpleFill
{
    typedef VaryingUV Varyings;
    RasterBuffer buffer;
    void operator()(Vertex pos, int length, VaryingUV v, VaryingUV dx) {
        unsigned int *dst = buffer.scanline((int) pos.y) + (int) pos.x;
        for (int i=0; i<length; ++i) {
            float n = stb_perlin_noise3(v.u, v.v, 0.0f, 0, 0, 0);
            if (n >= 0)
                dst[i] = 0xff000000;
            v = v + dx;
        }
    }
};

void doSimpleStroke()
{
    Stroker<LerpRaster<SimpleFill>, VaryingGeneratorLengthWidth> stroker;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->allocate(800, 800);
    buffer->fill(0xffffffff);

    const int COUNT = 100;

    time_point<std::chrono::system_clock> timeStart = system_clock::now();

    for (int i=0; i<COUNT; ++i) {

        float t = i / float(COUNT);

        float x = 100 + t * 600;
        float y = 600 + 100 * cos(4 * t * M_PI) - t * 400;

        stroker.width = 5 + 15 * std::abs(cos(4 * t * M_PI));

        if (i == 0) {
            stroker.moveTo(x, y);
        } else {
            stroker.lineTo(x, y);
        }
    }

    time_point<std::chrono::system_clock> timeEnd = system_clock::now();
    cout << "Simple stroke completed in: " << duration<double>(timeEnd - timeStart).count() << " seconds" << endl
         << " - length: " << stroker.length << " pixels.." << endl
         << " - triangles: " << stroker.triangleCount << endl
         << " - segments: " << COUNT << endl;


    stbi_write_png("strokelength_simple.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
}

int main(int argc, char **argv)
{
    doFbmStroke();

    doSimpleStroke();
}
