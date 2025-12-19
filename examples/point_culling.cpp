/*
    Copyright (c) 2025, reMarkable AS <technology@remarkable.no>
    Copyright (c) 2025, Gunnar Sletta <gunnar@crimson.no>
    All rights reserved.


    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

struct VarGen
{
    float a = 0.0f;
    float b = 0.0f;
    float lengthFactor = 0.01f;
    float widthFactor = 0.5f;

    Varying4D right(float length, float width) {
        return Varying4D(length * lengthFactor, -width * widthFactor, a, b);
    }

    Varying4D left(float length, float width) {
        return Varying4D(length * lengthFactor, width * widthFactor, a, b);
    }
};

struct Filler
{
    typedef Varying2D Varyings;
    RasterBuffer buffer;

    uint32_t colorMask = 0x00ff0000;

    void operator()(int x, int y, int length, Varying2D V, Varying2D dV) {
        uint32_t *dst = buffer.scanline(y) + x;
        for (int i=0; i<length; ++i) {
            if ((uint32_t(V.x / 10.0) & 0x1) == 0) {
                dst[i] |= colorMask;
            }
            V = V + dV;
        }
    }
};

static void doStroke(auto& stroker)
{
    constexpr int COUNT = 1000;
    constexpr int LOOPS = 3;
    constexpr float RADIUS = 300.0f;
    constexpr float CX = 400.0f;
    constexpr float CY = 400.0f;

    for (int i=0; i<COUNT; ++i) {
        const float r = RADIUS * float(i) / float(COUNT - 1);
        const float t = 2.0f * std::numbers::pi_v<float> * float(i) / float(COUNT / LOOPS);
        const float x = CX + r * cos(t);
        const float y = CY + r * sin(t);
        stroker.lineTo(x, y);
    }

    stroker.finish();
}

int main(int argc, char **argv)
{
    Stroker<ClipRaster<LerpRaster<Filler>>, VaryingGeneratorLengthWidth> stroker;
    stroker.width = 10;

    auto& clipper = stroker.raster;
    auto& filler = clipper.raster.fill;

    filler.colorMask = 0x00ff0000;

    RasterBuffer *buffer = &filler.buffer;
    buffer->allocate(800, 800);
    buffer->fill(0xff000000);

    stroker.raster.x0 = 0;
    stroker.raster.x1 = 800;
    stroker.raster.y0 = 0;
    stroker.raster.y1 = 800;

    // time_point<std::chrono::system_clock> timeStart = system_clock::now();
    doStroke(stroker);
    // time_point<std::chrono::system_clock> timeEnd = system_clock::now();
    stroker.minx = 0;
    stroker.maxx = 400;
    stroker.miny = 0;
    stroker.maxy = 400;
    stroker.raster.x0 = 0;
    stroker.raster.x1 = 400;
    stroker.raster.y0 = 0;
    stroker.raster.y1 = 400;

    filler.colorMask = 0x000000ff;

    doStroke(stroker);
    stbi_write_png("point_culling.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
    std::cout << "wrote 'point_culling.png'..." << std::endl;

    std::cout << "running benchmark:" << std::endl;

    constexpr uint32_t iterations = 100000;

    {
        const auto timeStart = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            doStroke(stroker);
        }
        const auto timeCulled = std::chrono::duration<double>(std::chrono::steady_clock::now() - timeStart);
        std::cout << " - culled ....: " << timeCulled.count() << std::endl;
    }

    {
        stroker.minx = 0;
        stroker.maxx = 800;
        stroker.miny = 0;
        stroker.maxy = 800;
        const auto timeStart = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            doStroke(stroker);
        }
        const auto timeCulled = std::chrono::duration<double>(std::chrono::steady_clock::now() - timeStart);
        std::cout << " - unculled ....: " << timeCulled.count() << std::endl;
    }

    // cout << "Simple stroke completed in: " << duration<double>(timeEnd - timeStart).count() << " seconds" << endl;

}
