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



template <typename FillFunction>
void LerpRaster<FillFunction>::operator()(Triangle t, Varyings va, Varyings vb, Varyings vc)
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

    float y;
    float yaFloored = std::floor(t.a.y);
    if (t.a.y - yaFloored < 0.5) {
        y = yaFloored + 0.5;
    } else {
        y = yaFloored + 1.5;
    }

    float yb;
    float ybFloored = std::floor(t.b.y);
    if (t.b.y - ybFloored < 0.5) {
        yb = ybFloored - 0.5;
    } else {
        yb = ybFloored + 0.5;
    }

    float yc;
    float ycFloored = std::floor(t.c.y);
    if (t.c.y - ycFloored < 0.5) {
        yc = ycFloored - 0.5;
    } else {
        yc = ycFloored + 0.5;
    }

    /*
     *
     *         A
     *       /   \
     *      /     \
     *     /       \
     *    /         \
     *   B   -   -   ACB
     *     --__       \
     *         --__    \
     *             --__ \
     *                 --C
     */

    // std::cout << "rasterizing: " << t << " " << va << vb << vc << std::endl;
    // std::cout << " - y=" << y << ", yaFloored=" << yaFloored << std::endl;

    if (y <= yb) {
        float dyab = (t.b.y - t.a.y);
        float dyac = (t.c.y - t.a.y);

        float dxl = (t.b.x - t.a.x) / dyab;
        float dxr = (t.c.x - t.a.x) / dyac;
        Varyings dxld = (vb - va) / dyab;
        Varyings dxrd = (vc - va) / dyac;

        if (dxr < dxl) {
            std::swap(dxl, dxr);
            std::swap(dxld, dxrd);
        }

        float height = yb - t.a.y;

        float bx = t.a.x + dxl * height;
        float acbx = t.a.x + dxr * height;
        float wb = acbx - bx;

        Varyings varB = va + dxld * height;
        Varyings varACB = va + dxrd * height;
        Varyings varDX = (varACB - varB) / wb;

        float yoffset = y - t.a.y;
        float left = t.a.x + dxl * yoffset;
        float right = t.a.x + dxr * yoffset;
        Varyings varLeft = va + dxld * yoffset;

        while (y <= yb) {
            float l = std::ceil(left - 0.5) + 0.5;
            float r = std::floor(right + 0.5) - 0.5;

            if (r >= l) {
                Varyings var = varLeft + varDX * (l - left);
                fill(Vertex(l, y), int(r - l) + 1, var, varDX);
            }

            left += + dxl;
            right +=  + dxr;
            ++y;
            varLeft = varLeft + dxld;
        }
    }

    if (y <= yc) {
        float dyac = (t.c.y - t.a.y);
        float dybc = (t.c.y - t.b.y);

        float dxl = (t.c.x - t.b.x) / dybc;
        float dxr = (t.c.x - t.a.x) / dyac;
        Varyings dxld = (vc - vb) / dybc;
        Varyings dxrd = (vc - va) / dyac;

        if (dxl < dxr) {
            std::swap(dxl, dxr);
            std::swap(dxld, dxrd);
        }

        float height = t.c.y - y;
        float left = t.c.x - dxl * height;
        float right = t.c.x - dxr * height;
        float width = right - left;

        // std::cout << "2nd-half: y=" << y << ", height=" << height << ", dyac=" << dyac
        //           << ", dybc=" << dybc << ", dxr=" << dxr << ", dxl=" << dxl << ", leftx=" << left << ", right=" << right
        //           << ", width=" << width << std::endl;

        Varyings varLeft = vc - dxld * height;
        Varyings varRight = vc - dxrd * height;

        Varyings varDX = (varRight - varLeft) / width;

        // std::cout << "          dxld=" << dxld << ", dxrd=" << dxrd << ", varLeft=" << varLeft << ", varRight=" << varRight << ", varDX=" << varDX << endl;

        while (y <= yc) {
            float l = std::ceil(left - 0.5) + 0.5;
            float r = std::floor(right + 0.5) - 0.5;

            if (r >= l) {
                Varyings var = varLeft + varDX * (l - left);
                fill(Vertex(l, y), int(r - l) + 1, var, varDX);
            }

            left += + dxl;
            right +=  + dxr;
            ++y;
            varLeft = varLeft + dxld;
        }
    }

}
