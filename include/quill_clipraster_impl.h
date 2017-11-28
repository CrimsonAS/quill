/*
    Copyright (c) 2017, Gunnar Sletta <gunnar@crimson.no>
    Copyright (c) 2017, reMarkable AS <technology@remarkable.no>
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

template <typename Raster>
void ClipRaster<Raster>::operator()(Triangle t, Varyings a, Varyings b, Varyings c)
{
    ySort(t, a, b, c);

    // Check for fully outside
    if (t.c.y < y0 || t.a.y > y1)
        return;

    if (t.a.x < x0 || t.b.x < x0 || t.c.x < x0
        || t.a.x > x1 || t.b.x > x1 || t.c.x > x1
        || t.a.y < y0 || t.b.y < y0 || t.c.y < y0
        || t.a.y > y1 || t.b.y > y1 || t.c.y > y1) {
        std::cout << "clipping: " << t << " to: x=[" << x0 << "->" << x1 << "], y=[" << y0 << "->" << y1 << "]" << std::endl;
        clipTop(t, a, b, c);
    //     // ### Segment the triangle into chunks and call fill with
    //     // the splitted triangles.
        return;
    }

    raster(t, a, b, c);
}

template <typename Raster>
void ClipRaster<Raster>::ySort(Triangle &t, Varyings &va, Varyings &vb, Varyings &vc)
{
    // Sort the triangle and the varyings
    if (t.b.y < t.a.y) {
        std::swap(t.a, t.b);
        std::swap(va, vb);
    }
    if (t.c.y < t.a.y) {
        std::swap(t.a, t.c);
        std::swap(va, vc);
    }
    if (t.c.y < t.b.y) {
        std::swap(t.c, t.b);
        std::swap(vc, vb);
    }
}

template <typename Raster>
void ClipRaster<Raster>::clipTop(Triangle t, Varyings a, Varyings b, Varyings c)
{
    assert(t.a.y <= t.b.y); // should already be sorted up above
    assert(t.b.y <= t.c.y);

    std::cout << " - clipping top, t=" << t << ", a=" << a << ", b=" << b << ", c=" << c << ", y0=" << y0 << std::endl;

    if (t.a.y >= y0) {
        std::cout << " - clip top 1" << std::endl;
        clipBottom(t, a, b, c);
    } else if (t.b.y > y0) {
        std::cout << " - clip top 2" << std::endl;
        float dy = y0 - t.a.y;
        float dab = dy / (t.b.y - t.a.y);
        float dac = dy / (t.c.y - t.a.y);
        Varyings vab = a + (b - a) * dab;
        Varyings vac = a + (c - a) * dac;
        float xab = t.a.x + (t.b.x - t.a.x) * dab;
        float xac = t.a.x + (t.c.x - t.a.x) * dac;
        clipBottom(Triangle(Vertex(xab, y0), Vertex(xac, y0), t.b), vab, vac, b);
        clipBottom(Triangle(Vertex(xac, y0), t.b, t.c), vac, b, c);
    } else {
        std::cout << " - clip top 3" << std::endl;
        float dy = y0 - t.c.y;
        float dca = dy / (t.a.y - t.c.y);
        float dcb = dy / (t.b.y - t.c.y);
        Varyings vca = c + (a - c) * dca;
        Varyings vcb = c + (b - c) * dcb;
        float xca = t.c.x + (t.a.x - t.c.x) * dca;
        float xcb = t.c.x + (t.b.x - t.c.x) * dcb;
        clipBottom(Triangle(Vertex(xca, y0), Vertex(xcb, y0), t.c), vca, vcb, c);
    }

    std::cout << " <- end of clip, t=" << t << std::endl;
}

template <typename Raster>
void ClipRaster<Raster>::clipBottom(Triangle t, Varyings a, Varyings b, Varyings c)
{
    assert(t.a.y <= t.b.y); // we should continue to generate sorted triangles
    assert(t.b.y <= t.c.y);

    std::cout << " -- clipping bottom, t=" << t << ", a=" << a << ", b=" << b << ", c=" << c << std::endl;
    if (t.c.y < y1) {
        clipLeft(t, a, b, c);

    } else if (t.b.y > y1) {

    } else {

    }
}

template <typename Raster>
void ClipRaster<Raster>::clipLeft(Triangle t, Varyings a, Varyings b, Varyings c)
{
    clipRight(t, a, b, c);
}

template <typename Raster>
void ClipRaster<Raster>::clipRight(Triangle t, Varyings a, Varyings b, Varyings c)
{
    raster(t, a, b, c);
}
