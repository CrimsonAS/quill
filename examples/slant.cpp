/*
    Copyright (c) 2023, reMarkable AS <technology@remarkable.no>
    Copyright (c) 2023, Gunnar Sletta <gunnar@crimson.no>
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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "rasterbuffer.h"
#include "solidfillstroker.h"

using namespace Quill;
using namespace std;

struct Fill
{
    using Varyings = Varying2D;
    RasterBuffer buffer;
    void operator()(int x, int y, unsigned int length, Varying2D V, Varying2D dV)
    {
        int x0 = x;
        int x1 = x + length;
        unsigned int *dst = buffer.scanline(y);
        for (int x = x0; x < x1; ++x) {
#if 1
            const float green = std::min(1.0f, V.x / 1000.0f);
            float blue = std::abs(V.y / 40.0f) * 255.0f;
            dst[x] = dst[x]
                | (uint32_t(255 * green) << 8)
                | (uint32_t(blue) << 16);
            V = V + dV;
#else
            dst[x] += 0x404040;
#endif
        }
    }
};

int main(int argc, char **argv)
{
    RasterBuffer buffer;
    buffer.allocate(800, 800);
    buffer.fill(0xff000000);

    Stroker<LerpRaster<Fill>, VaryingGeneratorLengthWidth> stroker;
    stroker.raster.fill.buffer.reference(buffer);

    const float angle = M_PI * 2.5f / 4.0f;

    Slant slant {
        .nx = std::cos(angle),
        .ny = -std::sin(angle),
        .minimumWidth = 20.0f
    };
    stroker.slant = std::make_optional<Slant>(slant);

    stroker.width = 80;

    const int COUNT = 100;
    for (int i=0; i<COUNT; ++i) {
        const float t = i / float(COUNT);
        const float x = 100 + t * 600;
        const float y = 600 + 100 * cos(4 * t * M_PI) - t * 400;

        // stroker.slant->nx = std::cos(t * M_PI);
        // stroker.slant->ny = std::sin(t * M_PI);

        stroker.lineTo(x, y);
    }

    cout << "Simple stroke completed:" << endl
         << " - length: " << stroker.length << " pixels.." << endl
         << " - triangles: " << stroker.triangleCount << endl;

    stbi_write_png("slant.png",
                   buffer.width,
                   buffer.height,
                   4,
                   buffer.data,
                   buffer.width * sizeof(unsigned int));

    cout << " - wrote 'slant.png'" << endl;
}


