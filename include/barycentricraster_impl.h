/*
    Copyright (c) 2017, Gunnar Sletta <gunnar@crimson.no>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

// Implementation from: https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/

template <typename FillFunction>
int BarycentricRaster<FillFunction>::orient2d(int ax, int ay, int bx, int by, int cx, int cy) const
{
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

template <typename FillFunction>
void BarycentricRaster<FillFunction>::operator()(Triangle t, Varyings, Varyings, Varyings)
{
    int minX = std::min(t.a.x, std::min(t.b.x, t.c.x));
    int minY = std::min(t.a.y, std::min(t.b.y, t.c.y));
    int maxX = std::max(t.a.x, std::max(t.b.x, t.c.x));
    int maxY = std::max(t.a.y, std::max(t.b.y, t.c.y));

    // Clip against screen bounds
    minX = std::max(minX, x0);
    minY = std::max(minY, y0);
    maxX = std::min(maxX, x1-1);
    maxY = std::min(maxY, y1-1);

    int A01 = t.a.y - t.b.y, B01 = t.b.x - t.a.x;
    int A12 = t.b.y - t.c.y, B12 = t.c.x - t.b.x;
    int A20 = t.c.y - t.a.y, B20 = t.a.x - t.c.x;

    // Barycentric coordinates at minX/minY corner
    int w0_row = orient2d(t.b.x, t.b.y, t.c.x, t.c.y, minX, minY);
    int w1_row = orient2d(t.c.x, t.c.y, t.a.x, t.a.y, minX, minY);
    int w2_row = orient2d(t.a.x, t.a.y, t.b.x, t.b.y, minX, minY);

    for (int y=minY; y<=maxY; ++y) {
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;

        int first = -1;
        int last = maxX;

        for (int x=minX; x<=maxX; ++x) {

            if ((w0 | w1 | w2) >= 0) {
                if (first < 0)
                    first = x;
            } else if (first >= 0) {
                last = x;
                break;
            }

            w0 += A12;
            w1 += A20;
            w2 += A01;
        }

        if (first >= 0) {
            fill(Vertex(first, y), last - first, Varyings(), Varyings());
        }

        w0_row += B12;
        w1_row += B20;
        w2_row += B01;
    }
}
